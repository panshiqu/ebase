/*
 * main.cpp
 *
 *  Created on: 2015年5月1日
 *      Author: root
 */

#include <signal.h>
#include "server.h"
#include "timer.h"

void signal_callback(int signo)
{
	cout << "signal callback." << endl;
}

void print_timer()
{
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
    printf("%d.%03d: ", secs, (nsecs + 500000) / 1000000);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, signal_callback);

//	server srv;
//	srv.init("127.0.0.1", 1234);
//	srv.loop();

	timer t;
	cout << "it is main" << endl;
	t.add_timer(print_timer, 5, 1);
	t.__start();
}

