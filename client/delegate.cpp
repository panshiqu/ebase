/*
 * delegate.cpp
 *
 *  Created on: 2015年6月30日
 *      Author: root
 */

#include "delegate.h"
#include "client.h"

using namespace std;

Delegate::Delegate()
	: _client(NULL)
	, _running(true)
{

}

Delegate::~Delegate()
{
	if (_client != NULL)
		delete _client;
}

void Delegate::onMessage(void)
{
	// 消息接收
	buffer &msgrecv = getRecv();
	do {
		int length = msgrecv.read_int();
		int command = msgrecv.read_int();
		int error = msgrecv.read_int();

		int len = length - HEADER_LENGTH;
		cout << "length: " << length << " command: " << command << " error: " << error << endl;

		// 增加起始偏移
		msgrecv.incr_start_offset(len);
	} while (msgrecv.check());

	msgrecv.reset();
}

void Delegate::onConnected(void)
{
	cout << "onConnected." << endl;
}

void Delegate::onDisconnected(void)
{
	cout << "onDisconnected." << endl;
}

void Delegate::onConnectFailed(void)
{
	cout << "onConnectFailed." << endl;
}

void Delegate::onConnectTimeout(void)
{
	cout << "onConnectTimeout." << endl;
}

bool Delegate::init(const char *address, int port)
{
	// 创建客户端
	_client = new Client(this);
	if (_client == NULL)
		return false;

	// 初始化服务
	if (!_client->init(address, port))
		return false;

	return true;
}

void Delegate::start(float interval)
{
	_client->loop(interval);
}

void Delegate::start(void)
{
	_client->loop();
}

buffer &Delegate::getRecv(void)
{
	return _client->getRecv();
}

buffer &Delegate::getSend(void)
{
	return _client->getSend();
}

