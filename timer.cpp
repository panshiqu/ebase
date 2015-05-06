/*
 * timer.cpp
 *
 *  Created on: 2015年5月6日
 *      Author: root
 */

#include "timer.h"

void print(void)
{
	cout << "hello world" << endl;
}

timer::timer()
{
	//add_timer(print, 10, 0);
	// 启动定时器线程
	//_thread = thread(&timer::__start, this);
}

timer::~timer()
{
	// 等待线程结束
	//_thread.join();

	// 清空定时器列表
	//_timers.clear();
}

void timer::__start(void)
{
	uint64_t timeouts;
	struct epoll_event events[NR_TIMER] = {0};

	while (true) {
		cout << "here is calla" << endl;
		//int n = epoll_wait(_epoller.fd(), events, NR_TIMER, 5000);
		recv(4, &timeouts, sizeof(uint64_t), 0);
		cout << "wo cao: " <<  timeouts << endl;
		//if (n == -1 && errno == EINTR) break;
		//cout << "here is callb - n: " << n << endl;

//		for (int i = 0; i < n; i++) {
//			recv(events[i].data.fd, &timeouts, sizeof(uint64_t), 0);
//			unique_lock<mutex> ulock(_mtx);
//			_timers[events[i].data.fd];
//		}
	}
}

bool timer::add_timer(const timercallback &tcb, time_t timeout, time_t interval)
{
	int timer;
	struct timespec now;
	struct itimerspec value;
	if (clock_gettime(CLOCK_MONOTONIC, &now) == -1) {
		cerr << "clock_gettime error." << endl;
		return false;
	}

	if ((timer = timerfd_create(CLOCK_MONOTONIC, 0)) == -1) {
		cerr << "timerfd_create error." << endl;
		return false;
	}

	cout << timer << endl;



	// 超时时间
	value.it_value.tv_sec = timeout;
	value.it_value.tv_nsec = 0;

	// 时间间隔
	if (interval) {
		value.it_interval.tv_sec = interval;
		value.it_interval.tv_nsec = 0;
	}

	now.tv_sec += timeout;

	if (timerfd_settime(timer, 0, &value, NULL) == -1) {
		cerr << "timerfd_settime error." << endl;
		return false;
	}

	// 更新MAP（加锁）
	unique_lock<mutex> ulock(_mtx);
	_timers.insert(make_pair(timer, tcb));
	// 增加监控
	_epoller.add(timer, EPOLLIN);

	return true;
}

bool timer::del_timer(int timer)
{
	if (timerfd_settime(timer, TFD_TIMER_ABSTIME, NULL, NULL) == -1) {
		cerr << "timerfd_settime error." << endl;
		return false;
	}

	// 移除监控并关闭
	_epoller.del(timer);
	close(timer);

	// 查找并移除（加锁）
	unique_lock<mutex> ulock(_mtx);
	map<int, timercallback>::iterator itr = _timers.find(timer);
	if (itr != _timers.end()) _timers.erase(itr);

	return true;
}

