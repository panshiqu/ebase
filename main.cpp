/*
 * main.cpp
 *
 *  Created on: 2015年5月1日
 *      Author: root
 */

#include <signal.h>
#include "server.h"

void signal_callback(int signo)
{
	cout << "signal callback." << endl;
}

int main(int argc, char *argv[])
{
	signal(SIGINT, signal_callback);

	server srv;
	srv.init("127.0.0.1", 1234);
	srv.loop();
}

