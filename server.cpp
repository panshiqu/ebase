/*
 * server.cpp
 *
 *  Created on: 2015年5月1日
 *      Author: root
 */

#include "server.h"
#include "client.h"

using namespace std;

server::server()
	: _listener(0)
{
	// 不考虑失败
	memset(fds, 0, sizeof(fds));
	socketpair(AF_LOCAL, SOCK_STREAM, 0, fds);
}

server::~server()
{
	for (int i = 0; i < NR_THREAD; i++)
		_threads[i].join();

	for (auto s : _clients)
		delete s.second;

	_clients.clear();
	close(_listener);
}

void server::__rscb(void)
{
	struct epoll_event events[NR_EVENT] = {0};

	while (true) {
		int n = epoll_wait(_receiver.fd(), events, NR_EVENT, -1);

		for (int i = 0; i < n; i++) {
			if (events[i].data.fd == fds[1])
			{
				exit_wait();
				return ;
			}

			client *pclient = (client *)events[i].data.ptr;
			if (__recv_send(pclient, events[i].events))
				__proc_mod(pclient);
			else
				__proc_err(pclient);
		}
	}
}

bool server::__nonblock(int sock)
{
	// 获取描述符属性
	int options = fcntl(sock, F_GETFL);
	if (options == -1) return false;
	options |= O_NONBLOCK;

	// 设置非阻塞属性
	if (fcntl(sock, F_SETFL, options) == -1) return false;
	return true;
}

void server::__proc_err(client *pclient)
{
	// 通知断开连接
	_disconnection(pclient);

	// 删除客户端
	__del_client(pclient->fd());

	/*
	 * 套接字此时已经出错
	 * 可能已经自动将其关闭
	 * 经验证此时是可以epoll_add
	 */
	//receiver.del(sock);
}

void server::__proc_mod(client *pclient)
{
	// 根据是否有需要发送内容进行修改
	int events = EPOLLIN | EPOLLONESHOT;
	if (pclient->is_need_send()) events |= EPOLLOUT;
	_receiver.mod(pclient->fd(), events, pclient);
}

bool server::__recv_send(client *pclient, int events)
{
	// always wait EPOLLERR, EPOLLHUP
	if ((events & EPOLLERR) || (events & EPOLLHUP))
		return false;

	// 收发套接字可以接收
	if ((events & EPOLLIN) && !pclient->run_recv())
		return false;

	// 收发套接字可以发送
	if ((events & EPOLLOUT) && !pclient->run_send())
		return false;

	return true;
}

client *server::__get_client(int sock)
{
	// 查找实例（加锁）
	unique_lock<mutex> ulock(_mutex);
	map<int, client *>::iterator itr = _clients.find(sock);
	if (itr == _clients.end()) return NULL;
	return itr->second;
}

client *server::__add_client(int sock)
{
	// 创建实例指针
	client *pclient = new client(sock, this);
	if (pclient == NULL) {
		LOG_ERROR << "new error.";
		return NULL;
	}

	// 更新MAP（加锁）
	unique_lock<mutex> ulock(_mutex);
	_clients.insert(make_pair(sock, pclient));
	return pclient;
}

bool server::__del_client(int sock)
{
	// 查找实例（加锁）
	unique_lock<mutex> ulock(_mutex);
	map<int, client *>::iterator itr = _clients.find(sock);
	if (itr == _clients.end()) return false;

	// 删除实例
	delete itr->second;
	_clients.erase(itr);
	return true;
}

bool server::init(const char *address, const int port)
{
	// 创建监听套接字
	if ((_listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		LOG_ERROR << "socket error.";
		return false;
	}

	// 设置可重用属性
	int reuseaddr = 1;
	if (setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) == -1) {
		LOG_ERROR << "reuseaddr error.";
		return false;
	}

	// 设置可重用属性
	int reuseport = 1;
	if (setsockopt(_listener, SOL_SOCKET, SO_REUSEPORT, &reuseport, sizeof(reuseport)) == -1) {
		LOG_ERROR << "reuseport error.";
		return false;
	}

	// 设置非阻塞属性
	if (!__nonblock(_listener)) {
		LOG_ERROR << "nonblock error.";
		return false;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, address, &(addr.sin_addr));

	// 绑定套接字
	if (bind(_listener, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		LOG_ERROR << "bind error.";
		return false;
	}

	// 监听套接字
	if (listen(_listener, 10) == -1) {
		LOG_ERROR << "listen error.";
		return false;
	}

	// 增加套接字
	if (!_accepter.add(_listener, EPOLLIN)) return false;

	// 启动线程
	for (int i = 0; i < NR_THREAD; i++)
		_threads[i] = thread(&server::__rscb, this);

	// 增加SocketPair
	_accepter.add(fds[1], EPOLLIN);
	_receiver.add(fds[1], EPOLLIN);

	return true;
}

void server::loop(void)
{
	struct epoll_event event = {0};

	while (true) {
		// 仅关注监听套接字（一个就够）(即便现在增加了SocketPair)
		epoll_wait(_accepter.fd(), &event, 1, -1);
		if (event.data.fd == fds[1]) break;

		while (true) {
			int sock;
			struct sockaddr_in addr;
			socklen_t length = sizeof(struct sockaddr_in);
			if ((sock = accept(_listener, (struct sockaddr *)&addr, &length)) == -1) {
				// 处理完所有连接请求 errno = EAGAIN
				if (errno != EAGAIN || errno != EWOULDBLOCK)
					LOG_ERROR << "accept error.";

				break;
			}

			// 设置非阻塞属性
			if (!__nonblock(sock)) {
				LOG_ERROR << "nonblock error.";
				break;
			}

			// 增加客户连接实例
			client *pclient = __add_client(sock);
			if (pclient == NULL) break;

			// 增加套接字（不管成功与否）
			_receiver.add(sock, EPOLLIN | EPOLLONESHOT, pclient);

			// 通知接受连接
			_connection(pclient);
		}
	}
}

void server::writable(int sock)
{
	// 修改进行检查套接字是否可写（给别人用）
	client *pclient = __get_client(sock);
	if (pclient != NULL) __proc_mod(pclient);
}

