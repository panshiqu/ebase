/*
 * server.h
 *
 *  Created on: 2015年5月1日
 *      Author: root
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <map>
#include <mutex>
#include <thread>
#include <iostream>
using namespace std;

#include "epoller.h"

#define NR_THREAD		4
#define NR_EVENTS		10
#define NR_TIMEOUT	1000

class client;
class server {
public:
	server();
	virtual ~server();

	/*
	 * receive thread callback
	 */
	void rcb(void);

	/*
	 * init listen socket
	 * epoll add listen socket
	 * multithread create
	 */
	bool init(const char *address, const int port);

	/*
	 * epoll wait for accept
	 */
	void loop(void);

	/*
	 * check writable
	 */
	void writable(int sock);

private:
	/*
	 * set nonblock option
	 */
	bool __nonblock(int sock);

	/*
	 * recv and send proc err
	 */
	void __proc_err(int sock);

	/*
	 * ensure events for receiver
	 */
	void __proc_mod(client *pclient);

	/*
	 * process recv and send
	 */
	bool __recv_send(client *pclient, int events);

	/*
	 * client map operation
	 */
	client *__get_client(int sock);
	client *__add_client(int sock);
	bool __del_client(int sock);

private:
	mutex _mtx;			// 互斥量
	int _listener;		// 监听套接字
	bool _running;		// 工作线程状态
	epoller _accepter;	// 仅处理连接
	epoller _receiver;	// 仅处理收发
	thread _threads[NR_THREAD];
	map<int, client *> _clients;
};

#endif /* SERVER_H_ */
