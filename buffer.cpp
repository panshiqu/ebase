/*
 * buffer.cpp
 *
 *  Created on: 2015年5月2日
 *      Author: root
 */

#include "buffer.h"

buffer::buffer()
	: _start_offset(0)
	, _valid_offset(0)
	, _total_length(0)
	, _buffer(NULL)
{
	// 初始化缓冲区长度
	_buffer = (char *)malloc(NR_BUFFER);
	_total_length = NR_BUFFER;
}

buffer::~buffer()
{
	// 释放缓冲区
	free(_buffer);
}

void buffer::append(char *buf, int len)
{
	// 记录位置刷新附加
	int pos = _valid_offset;
	refresh_recv(len);

	// 数据拷贝
	memcpy(&_buffer[pos], buf, len);
}

void buffer::refresh_recv(int len)
{
	_valid_offset += len;

	// 用尽缓冲区进行扩展
	if (_valid_offset == _total_length) {
		_total_length += NR_BUFFER;
		_buffer = (char *)realloc(_buffer, _total_length);
	}
}

void buffer::summarize_send(void)
{
	// 数据未发送完数据前移
	if (_start_offset != _valid_offset) {
		_valid_offset = get_send_length();
		memcpy(_buffer, &_buffer[_start_offset], _valid_offset);
	}
	// 数据发送完重置各偏移
	else _valid_offset = 0;

	// 无论如何
	_start_offset = 0;
}

