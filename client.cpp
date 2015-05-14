/*
 * client.cpp
 *
 *  Created on: 2015年5月2日
 *      Author: root
 */

#include "client.h"
#include "server.h"

client::client(int sock, server *srv)
	: _sfd(sock)
	, _srv(srv)
{

}

client::~client()
{
	close(_sfd);
}

bool client::run_recv(void)
{
	while (true) {
		int len;

		if ((len = recv(_sfd, _recv.get_valid_buffer(), _recv.get_idle_length(), 0)) == -1) {
			// 接收完所有数据 errno = EAGAIN
			if (errno != EAGAIN || errno != EWOULDBLOCK) {
				LOG_ERROR << "recv error.";
				return false;
			}

			break;
		}

		// 对端关闭连接
		if (len == 0) return false;

		// 增加有效偏移
		_recv.incr_valid_offset(len);
	}

	// 处理接收
	if (_recv.check())
		_srv->onmessage(this);

	return true;
}

bool client::run_send(void)
{
	while (_send.get_used_length() > 0) {
		int len;

		if ((len = send(_sfd, _send.get_start_buffer(), _send.get_used_length(), 0)) == -1) {
			// 接收完所有数据 errno = EAGAIN
			if (errno != EAGAIN || errno != EWOULDBLOCK) {
				LOG_ERROR << "send error.";
				return false;
			}

			break;
		}

		// 对端关闭连接
		if (len == 0) return false;

		// 更新缓冲区起始位置
		_send.incr_start_offset(len);
	}

	// 重置缓冲区
	_send.reset();

	return true;
}

