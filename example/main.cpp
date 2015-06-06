/*
 * main.cpp
 *
 *  Created on: 2015年5月13日
 *      Author: root
 */


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
using namespace std;

void write_int(int value, char *_buffer)
{
	_buffer[3] = (value & 0xFF);
	value = value >> 8;
	_buffer[2] = (value & 0xFF);
	value = value >> 8;
	_buffer[1] = (value & 0xFF);
	value = value >> 8;
	_buffer[0] = (value & 0xFF);
}

int main(int argc, char *argv[])
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr));
	connect(sock, (struct sockaddr *)&addr, sizeof(addr));

	while (true) {
		// 发送
		char buf[1024];
		write_int(13, buf);
		write_int(2147483647, &buf[4]);
		char hel[10] = "hello";
		memcpy(&buf[8], hel, 5);
		send(sock, buf, 13, 0);

		// 接收
		char rcv[1024] = {0};
		recv(sock, rcv, 1024, 0);
		cout << "Recv: " << rcv << endl;

		sleep(1);
	}
}

