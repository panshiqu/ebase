/*
 * client.h
 *
 *  Created on: 2015年5月2日
 *      Author: root
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <sys/types.h>
#include <sys/socket.h>

#include "buffer.h"

class server;
class client {
public:
	client(int sock, server *srv);
	virtual ~client();

	bool run_recv(void);
	bool run_send(void);
	void get_info(char *address, int &port);

	bool is_need_send(void)	{ return _send.get_valid_offset(); }
	buffer &get_recv(void)	{ return _recv; }
	buffer &get_send(void)	{ return _send; }
	int fd(void)	{ return _sfd; }

private:
	int _sfd;		// 套接字
	server *_srv;	// 服务器对象
	buffer _recv;	// 接收缓冲区
	buffer _send;	// 发送缓冲区
};

#endif /* CLIENT_H_ */
