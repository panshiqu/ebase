/*
 * timer.h
 *
 *  Created on: 2015年5月6日
 *      Author: root
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/timerfd.h>

#include <map>
#include <mutex>
#include <thread>
#include <iostream>

#include "epoller.h"
#include "logger.h"
#include "ebase.h"

typedef std::function<void (int)> timercallback;

class timer {
public:
	timer();
	virtual ~timer();

public:
	// run at assign time
	int run_at(const timercallback &tcb, time_t thetime);

	// run after a period of time
	int run_after(const timercallback &tcb, time_t timeout);

	// run every period of time
	int run_every(const timercallback &tcb, time_t interval);

	// delete timer
	void del_timer(int timer);

	// set default callback
	void set_callback(const timercallback &tcb)	{ _callback = tcb; }

	// exit epoll_wait to release
	void exit_wait(void)	{ write(fds[0], "exit", sizeof("exit")); }

private:
	void __start(void);
	timercallback &__get_timer(int timer);
	int __add_timer(const timercallback &tcb, time_t timeout, time_t interval);

private:
	int fds[2];				// SocketPair
	std::mutex _mutex;	// 定时器列表操作加锁
	std::thread _thread;	// 定时器线程
	epoller _epoller;		// 监控到期
	timercallback _callback;	// 默认回调
	std::map<int, timercallback> _timers;	// 定时器列表
};

#endif /* TIMER_H_ */
