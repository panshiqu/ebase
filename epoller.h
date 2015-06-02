/*
 * epoller.h
 *
 *  Created on: 2015年5月1日
 *      Author: root
 */

#ifndef EPOLLER_H_
#define EPOLLER_H_

#include <unistd.h>
#include <sys/epoll.h>

#include "logger.h"

class epoller {
public:
	epoller();
	virtual ~epoller();

	bool add(int fd, int events, void *ptr = NULL)
	{ return __eventctl(EPOLL_CTL_ADD, fd, events, ptr); }

	bool mod(int fd, int events, void *ptr = NULL)
	{ return __eventctl(EPOLL_CTL_MOD, fd, events, ptr); }

	bool del(int fd)
	{ return __ctl(EPOLL_CTL_DEL, fd, NULL); }

	int fd(void)
	{ return _efd; }

private:
	bool __eventctl(int type, int fd, int events, void *ptr = NULL);
	bool __ctl(int type, int fd, void *event);

private:
	int _efd;	// 描述符
};

#endif /* EPOLLER_H_ */
