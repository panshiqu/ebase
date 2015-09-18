/*
 * main.cpp
 *
 *  Created on: 2015年6月29日
 *      Author: root
 */


#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <thread>
using namespace std;

#include "delegate.h"
#include "buffer.h"

class NetDelegate : public Delegate
{
public:
	NetDelegate() {}
	virtual ~NetDelegate() {}

	void sendMessage(void)
	{
		buffer &msgsend = getSend();
		msgsend.write_int(12);
		msgsend.write_int(401);
		msgsend.write_int(0);
	}
};

NetDelegate delegate;

void signal_callback(int signo)
{
	cout << "signal callback." << endl;
	delegate.setRunning(false);
}

void th_fn(void)
{
	do {
		sleep(5);
		delegate.sendMessage();
	} while (delegate.getRunning());
}

int main(int argc, char *argv[])
{
	signal(SIGINT, signal_callback);

	thread t1{th_fn};

	if (delegate.init("127.0.0.1", 1234))
		delegate.start();

	t1.join();
}

