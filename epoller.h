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

#include <iostream>
using namespace std;

class epoller {
public:
	epoller();
	virtual ~epoller();

	bool add(int sock, int events, void *ptr = NULL)
	{ return __eventctl(EPOLL_CTL_ADD, sock, events, ptr); }

	bool mod(int sock, int events, void *ptr = NULL)
	{ return __eventctl(EPOLL_CTL_MOD, sock, events, ptr); }

	bool del(int sock)
	{ return __ctl(EPOLL_CTL_DEL, sock, NULL); }

	int fd(void)
	{ return _efd; }

private:
	bool __eventctl(int type, int sock, int events, void *ptr = NULL);
	bool __ctl(int type, int sock, void *event);

private:
	int _efd;
};

#endif /* EPOLLER_H_ */
