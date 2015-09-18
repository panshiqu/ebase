/*
 * client.h
 *
 *  Created on: 2015年6月29日
 *      Author: root
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#ifdef WIN32
#include <winsock2.h>
#else
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include "buffer.h"
#include "header.h"

class Delegate;
class Client {
public:
	Client(Delegate *delegate);
	virtual ~Client();

	enum SOCKET_STATUS {
		SOCKET_UNKNOWN				= 0,	// 未知
		SOCKET_CONNECTED			= 1,	// 已连接
		SOCKET_CONNECTING			= 2,	// 正在连接
		SOCKET_DISCONNECTED		= 3,	// 连接断开
		SOCKET_CONNECTFAILED		= 4,	// 连接失败
		SOCKET_CONNECTTIMEOUT	= 5,	// 连接超时
	};

	bool init(const char *address, int port);
	void loop(float interval);
	void loop(void);

	buffer &getRecv(void)	{ return _recv; }
	buffer &getSend(void)	{ return _send; }
	int getStatus(void)		{ return _status; }
	float getDuration(void)	{ return _duration; }
	void setStatus(int status)		{ _status = status; }
	void incrDuration(float duration){ _duration += duration; }

private:
	bool __connect(const char *address, int port);
	void __disconnect(void);

private:
	int _socket;		// 套接字描述符
	int _status;		// 套接字状态
	float _duration;	// 持续时间
	buffer _recv;		// 接收缓冲区
	buffer _send;		// 发送缓冲区
	Delegate *_delegate;	// 代理
};

#endif /* CLIENT_H_ */
