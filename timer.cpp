/*
 * timer.cpp
 *
 *  Created on: 2015年5月6日
 *      Author: root
 */

#include "timer.h"

timer::timer()
{
	// 启动定时器线程
	_thread = thread(&timer::__start, this);
}

timer::~timer()
{
	// 等待线程结束
	_thread.join();

	// 关闭定时器
	for (auto s : _timers)
		close(s.first);

	_timers.clear();
}

void timer::__start(void)
{
	/*
	 * 使用阻塞描述符
	 * 使用边缘触发方式
	 */

	uint64_t timeouts = 0;
	struct epoll_event events[NR_EVENT] = {0};

	while (true) {
		int n = epoll_wait(_epoller.fd(), events, NR_EVENT, -1);
		if (n == -1 && errno == EINTR) break;

		for (int i = 0; i < n; i++) {
			int timer = events[i].data.fd;
			read(timer, &timeouts, sizeof(uint64_t));
			__get_timer(timer)(timer);
		}
	}
}

timercallback &timer::__get_timer(int timer)
{
	// 查找并返回（加锁）
	unique_lock<mutex> ulock(_mutex);
	map<int, timercallback>::iterator itr = _timers.find(timer);
	if (itr != _timers.end()) return itr->second;
	return _callback;
}

int timer::__add_timer(const timercallback &tcb, time_t timeout, time_t interval)
{
	int timer;
	struct timespec now;
	struct itimerspec value = {0};
	if ((timer = timerfd_create(CLOCK_REALTIME, 0)) == -1) {
		LOG_ERROR << "timerfd_create error.";
		return false;
	}

	// 增加监控
	_epoller.add(timer, EPOLLIN);

	if (clock_gettime(CLOCK_REALTIME, &now) == -1) {
		LOG_ERROR << "clock_gettime error.";
		return false;
	}

	// 超时时间
	value.it_value.tv_sec = now.tv_sec + timeout;
	value.it_value.tv_nsec = now.tv_nsec;

	// 时间间隔
	if (interval) {
		value.it_interval.tv_sec = interval;
		value.it_interval.tv_nsec = 0;
	}

	if (timerfd_settime(timer, TFD_TIMER_ABSTIME, &value, NULL) == -1) {
		LOG_ERROR << "timerfd_settime error.";
		return false;
	}

	// 更新MAP（加锁）
	unique_lock<mutex> ulock(_mutex);
	_timers.insert(make_pair(timer, tcb));

	return timer;
}

int timer::run_at(const timercallback &tcb, time_t thetime)
{
	time_t timeout = thetime - time(NULL);
	return __add_timer(tcb, timeout, 0);
}

int timer::run_after(const timercallback &tcb, time_t timeout)
{
	return __add_timer(tcb, timeout, 0);
}

int timer::run_every(const timercallback &tcb, time_t interval)
{
	return __add_timer(tcb, interval, interval);
}

void timer::del_timer(int timer)
{
	// 移除监控并关闭
	_epoller.del(timer);
	close(timer);

	// 查找并移除（加锁）
	unique_lock<mutex> ulock(_mutex);
	map<int, timercallback>::iterator itr = _timers.find(timer);
	if (itr != _timers.end()) _timers.erase(itr);
}

