/*
 * client.cpp
 *
 *  Created on: 2015年6月29日
 *      Author: root
 */

#include "client.h"
#include "delegate.h"

#ifdef WIN32
static bool wsaStartup = false;
#endif

Client::Client(Delegate *delegate)
	: _socket(-1)
	, _status(0)
	, _duration(0.0)
	, _delegate(delegate)
{
#ifdef WIN32
	if (!wsaStartup)
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 0), &wsaData);
		wsaStartup = true;
	}
#endif
}

Client::~Client()
{
	if (_socket != -1)
	{
#ifdef WIN32
		shutdown(_socket, SD_BOTH);
		closesocket(_socket);
#else
		shutdown(_socket, SHUT_RDWR);
		close(_socket);
#endif
	}
}

bool Client::init(const char *address, int port)
{
	// 创建套接字
	if ((_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		return false;

#ifdef WIN32
	// 设置非阻塞属性
	unsigned long nonblock = 1;
	if (ioctlsocket(_socket, FIONBIO, (unsigned long *)&nonblock) == SOCKET_ERROR)
		return false;
#else
	// 获取描述符属性
	int options = 0;
	if ((options = fcntl(_socket, F_GETFL)) == -1)
		return false;

	// 设置非阻塞属性
	options |= O_NONBLOCK;
	if (fcntl(_socket, F_SETFL, options) == -1)
		return false;
#endif

	// 连接服务器
	if (!__connect(address, port))
		return false;

	return true;
}

void Client::loop(float interval)
{
	fd_set rfds, wfds;
	struct timeval tv = {0};

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);

	if (getStatus() == SOCKET_CONNECTED) FD_SET(_socket, &rfds);
	if (_send.get_valid_offset() || getStatus() == SOCKET_CONNECTING) FD_SET(_socket, &wfds);
	while ((select(_socket+1, &rfds, &wfds, NULL, &tv) == -1) && (errno == EINTR));

	if (FD_ISSET(_socket, &rfds))
	{
		while (true) {
			int len;

			while (((len = recv(_socket, (char *)_recv.get_valid_buffer(), _recv.get_idle_length(), 0)) == -1) && (errno == EINTR));
#ifdef WIN32
			DWORD dwError = GetLastError();
			if (len == -1 && (dwError == EAGAIN || dwError == WSAEWOULDBLOCK)) break;
#else
			if (len == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) break;
#endif
			if (len <= 0)	{ __disconnect(); break; }

			// 增加有效偏移
			_recv.incr_valid_offset(len);
		}

		// 处理接收
		if (_recv.check())
			_delegate->onMessage();
	}

	if (FD_ISSET(_socket, &wfds))
	{
		if (getStatus() == SOCKET_CONNECTING)
		{
// CocosNet
#ifdef WIN32
			// 连接成功
			setStatus(SOCKET_CONNECTED);

			// 通知代理
			_delegate->onConnected();
#else
			// 连接状态
			int error;
			socklen_t length = sizeof(error);
			if (getsockopt(_socket, SOL_SOCKET, SO_ERROR, (char *)&error, &length) != -1
				&& error != ECONNREFUSED && error != ETIMEDOUT)
			{
				// 连接成功
				setStatus(SOCKET_CONNECTED);

				// 通知代理
				_delegate->onConnected();
			}
			else
			{
				// 连接拒绝或超时
				setStatus(SOCKET_CONNECTFAILED);

				// 通知代理
				_delegate->onConnectFailed();
				_delegate->setRunning(false);
			}
#endif
		}

		while (_send.get_used_length() > 0) {
			int len;

			while (((len = send(_socket, (char *)_send.get_start_buffer(), _send.get_used_length(), 0)) == -1) && (errno == EINTR));
#ifdef WIN32
			DWORD dwError = GetLastError();
			if (len == -1 && (dwError == EAGAIN || dwError == WSAEWOULDBLOCK)) break;
#else
			if (len == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) break;
#endif
			if (len <= 0)	{ __disconnect(); break; }

			// 更新缓冲区起始位置
			_send.incr_start_offset(len);
		}

		// 重置缓冲区
		_send.reset();
	}

	// 这段逻辑没有实际的方法测试？
	if (getStatus() == SOCKET_CONNECTING)
	{
		if (getDuration() > NR_DURATION)
		{
			// 连接超时
			setStatus(SOCKET_CONNECTTIMEOUT);

			// 通知代理
			_delegate->onConnectTimeout();
			_delegate->setRunning(false);
		}
		else
		{
			incrDuration(interval);
		}
	}
}

void Client::loop(void)
{
	// 自建轮询
	while (_delegate->getRunning()) {
		loop(NR_INTERVAL);
#ifdef WIN32
		Sleep(NR_INTERVAL/1000);
#else
		usleep(NR_INTERVAL);
#endif
	}
}

bool Client::__connect(const char *address, int port)
{
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
#ifdef WIN32
	addr.sin_addr.S_un.S_addr = inet_addr(address);
#else
	inet_pton(AF_INET, address, &(addr.sin_addr));
#endif

	// 连接服务器
	if ((connect(_socket, (struct sockaddr *)&addr, sizeof(addr)) != 0)
#ifdef WIN32
		&& WSAGetLastError() != WSAEWOULDBLOCK)
#else
		&& errno != EINPROGRESS)
#endif
		return false;

	// 正在连接
	setStatus(SOCKET_CONNECTING);
	return true;
}

void Client::__disconnect(void)
{
	// 出错或对端关闭
	setStatus(SOCKET_DISCONNECTED);

	// 通知代理
	_delegate->onDisconnected();
	_delegate->setRunning(false);
}

