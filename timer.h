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
#include <functional>
using namespace std;

#include "epoller.h"
#include "ebase.h"

typedef function<void (int)> timercallback;

class timer {
public:
	timer(const timercallback &tcb);
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

private:
	void __start(void);
	timercallback &__get_timer(int timer);
	int __add_timer(const timercallback &tcb, time_t timeout, time_t interval);

private:
	mutex _mutex;
	thread _thread;
	epoller _epoller;
	timercallback _callback;
	map<int, timercallback> _timers;
};

#endif /* TIMER_H_ */
