/*
 * main.cpp
 *
 *  Created on: 2015年5月1日
 *      Author: root
 */

#include <signal.h>

#include "logger.h"
#include "server.h"
#include "client.h"
#include "buffer.h"
#include "timer.h"

//timer t;	// 全局

struct tag {
	int length;
};

void signal_callback(int signo)
{
	LOG_INFO << "signal callback.";
	exit(EXIT_SUCCESS);
}

void default_timer(int timer)
{
	LOG_INFO << "catch error on " << timer;
}

void default_onmessage(client *pclient)
{
	LOG_INFO << "default_onmessage.";
}

void default_connection(client *pclient)
{
	LOG_INFO << "default_connection.";
	buffer &msg = pclient->get_recv();
	msg.write_int(4160486910);
	msg.read_int();
}

void default_disconnection(client *pclient)
{
	LOG_INFO << "default_disconnection.";
}

void print_timer(int timer)
{
	// please see "man timer_create"
	static struct timespec start;
	struct timespec curr;
	static int first_call = 1;
	int secs, nsecs;

	if (first_call) {
		first_call = 0;
		clock_gettime(CLOCK_MONOTONIC, &start);
	}

	clock_gettime(CLOCK_MONOTONIC, &curr);

	secs = curr.tv_sec - start.tv_sec;
	nsecs = curr.tv_nsec - start.tv_nsec;

	if (nsecs < 0) {
		secs--;
		nsecs += 1000000000;
	}

	LOG_INFO << secs << "." << nsecs;
}

int main(int argc, char *argv[])
{
	signal(SIGINT, signal_callback);

	// 初始化日志模块
	logmgr::ins().init("/root/log");
	logmgr::ins().set_type(LOG_CONSOLE | LOG_FILE);
	logmgr::ins().set_level(logmgr::LOG_LEVEL::TRACE);

	// 新增定时任务
//	t.run_every(print_timer, 10);

	// 服务器
	server srv;
	srv.set_onmessage(default_onmessage);
	srv.set_connection(default_connection);
	srv.set_disconnection(default_disconnection);
	srv.init("127.0.0.1", 1234);
	srv.loop();
}

