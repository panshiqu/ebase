/*
 * delegate.h
 *
 *  Created on: 2015年6月30日
 *      Author: root
 */

#ifndef DELEGATE_H_
#define DELEGATE_H_

#include <iostream>

class buffer;
class Client;
class Delegate {
public:
	Delegate();
	virtual ~Delegate();

	virtual void onMessage(void);

	virtual void onConnected(void);

	virtual void onDisconnected(void);

	virtual void onConnectFailed(void);

	virtual void onConnectTimeout(void);

	bool init(const char *address, int port);
	void start(float interval);
	void start(void);

	buffer &getRecv(void);
	buffer &getSend(void);
	bool getRunning(void)	{ return _running; }
	void setRunning(bool running)	{ _running = running; }

private:
	Client *_client;	// 客户端
	bool _running;		// 是否运行
};

#endif /* DELEGATE_H_ */
