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

typedef function<void ()> timercallback;

#define NR_TIMER 10

class timer {
public:
	timer();
	virtual ~timer();

	void __start(void);


	bool add_timer(const timercallback &tcb, time_t timeout, time_t interval);
	bool del_timer(int timer);

private:
	mutex _mtx;
	thread _thread;
	epoller _epoller;
	map<int, timercallback> _timers;
};

#endif /* TIMER_H_ */
