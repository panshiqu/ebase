/*
 * epoller.cpp
 *
 *  Created on: 2015年5月1日
 *      Author: root
 */

#include "epoller.h"

epoller::epoller()
	: _efd(0)
{
	// 不考虑失败
	_efd = epoll_create1(0);
}

epoller::~epoller()
{
	// 关闭
	close(_efd);
}

bool epoller::__eventctl(int type, int fd, int events, void *ptr)
{
	struct epoll_event event = {0};
	event.events = events | EPOLLET;

	if (ptr == NULL) event.data.fd = fd;
	else event.data.ptr = ptr;

	return __ctl(type, fd, &event);
}

bool epoller::__ctl(int type, int fd, void *event)
{
	if (epoll_ctl(_efd, type, fd, (struct epoll_event *)event) == -1) {
		LOG_ERROR << "epoll_ctl error.";
		return false;
	}

	return true;
}

