/*
 * main.cpp
 *
 *  Created on: 2015年5月1日
 *      Author: root
 */

#include <signal.h>
#include "server.h"
#include "timer.h"
#include "logger.h"

void signal_callback(int signo)
{
	cout << "signal callback." << endl;
}

void error_timer(int timer)
{
	cout << "catch error on " << timer << endl;
}

timer t(error_timer);

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
   // printf("%d.%03d: \n", secs, (nsecs + 500000) / 1000000);
    printf("%d.%09d: \n", secs, nsecs);

//    if (secs > 2) return t.del_timer(timer);
}

void time1(int timer)
{
	cout << "time1" << endl;
}

void time2(int timer)
{
	cout << "time2" << endl;
}

void time3(int timer)
{
	cout << "time3" << endl;
}

int main(int argc, char *argv[])
{
//	signal(SIGINT, signal_callback);

//	server srv;
//	srv.init("127.0.0.1", 1234);
//	srv.loop();

//	time_t now = time(NULL);
//	t.run_at(time1, now+5);
//	t.run_after(time2, 10);
//	t.run_every(time3, 1);
//	t.__start();

//	logger::ins().set_level(logger::LOG_LEVEL::TRACE);
//	LOG_TRACE << "hello";
//	LOG_ERROR << "hello";
//
//	logger::ins().print();
}

