/*
 * server.h
 *
 *  Created on: 2015年5月1日
 *      Author: root
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <map>
#include <mutex>
#include <thread>
#include <iostream>

#include "epoller.h"
#include "logger.h"
#include "ebase.h"

class client;
typedef std::function<void (client *)> onmessagecallback;
typedef std::function<void (client *)> connectioncallback;
typedef std::function<void (client *)> disconnectioncallback;

class server {
public:
	server();
	virtual ~server();

	/*
	 * init listen socket
	 * epoll add listen socket
	 * multithread create
	 */
	bool init(const char *address, const int port);

	/*
	 * epoll wait for accept
	 */
	void loop(void);

	/*
	 * check writable
	 */
	void writable(int sock);

	/*
	 * server callback etc
	 */
	void onmessage(client *pclient)	{ _onmessage(pclient); }
	void set_onmessage(const onmessagecallback &cb)	{ _onmessage = cb; }
	void set_connection(const connectioncallback &cb)	{ _connection = cb; }
	void set_disconnection(const disconnectioncallback &cb)	{ _disconnection = cb; }

	// exit epoll_wait to release
	void exit_wait(void)	{ write(_fds[0], "exit", sizeof("exit")); }

private:
	/*
	 * receive send thread callback
	 */
	void __rscb(void);

	/*
	 * set nonblock option
	 */
	bool __nonblock(int sock);

	/*
	 * recv and send proc err
	 */
	void __proc_err(client *pclient);

	/*
	 * ensure events for receiver
	 */
	void __proc_mod(client *pclient);

	/*
	 * process recv and send
	 */
	bool __recv_send(client *pclient, int events);

	/*
	 * client map operation
	 */
	client *__get_client(int sock);
	client *__add_client(int sock);
	bool __del_client(int sock);

private:
	int _fds[2];		// SocketPair
	int _listener;		// 监听套接字
	epoller _accepter;	// 仅处理连接
	epoller _receiver;	// 仅处理收发
	std::mutex _mutex;		// 互斥量
	std::thread _threads[NR_THREAD];	// 工作线程
	std::map<int, client *> _clients;// 客户端实例
	onmessagecallback _onmessage;	// 接收消息回调
	connectioncallback _connection;	// 接受连接回调
	disconnectioncallback _disconnection;	// 断开连接回调
};

#endif /* SERVER_H_ */
