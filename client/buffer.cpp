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
	_buffer = (unsigned char *)malloc(NR_BUFFER);
	_total_length = NR_BUFFER;
}

buffer::~buffer()
{
	// 释放缓冲区
	free(_buffer);
}

void buffer::extra(void)
{
	// 用尽缓冲区进行扩展
	while (_valid_offset >= _total_length) {
		_total_length += NR_BUFFER;
		_buffer = (unsigned char *)realloc(_buffer, _total_length);
	}
}

void buffer::reset(void)
{
	// 数据未使用完数据前移
	if (_start_offset != _valid_offset) {
		_valid_offset = get_used_length();
		memcpy(_buffer, &_buffer[_start_offset], _valid_offset);
	}
	// 数据使用完重置各偏移
	else _valid_offset = 0;

	// 无论如何
	_start_offset = 0;
}

bool buffer::check(void)
{
	int len = sizeof(int);

	// 检测是否收全长度值
	if (get_used_length() < len)
		return false;

	// 读取消息长度并回退偏移
	int length = read_int();
	decr_start_offset(len);

	// 检测是否收全信息
	if (length > get_used_length())
		return false;

	return true;
}

void buffer::ensure(int len)
{
	// 确保缓冲区足够使用
	while (get_idle_length() <= len) {
		_total_length += NR_BUFFER;
		_buffer = (unsigned char *)realloc(_buffer, _total_length);
	}
}

int buffer::read_int(void)
{
	// 循环以BYTE位的方式填充整形
	int value = _buffer[_start_offset++];
	for (size_t i = 1; i < sizeof(int); i++) {
		value = value << 8;
		value |= _buffer[_start_offset++];
	}

	return value;
}

void buffer::write_int(int value)
{
	ensure(sizeof(int));

	// 循环以BYTE位的方式写入缓存
	for (int i = sizeof(int)-1; i >= 0; i--)
		_buffer[_valid_offset++] = ((value >> (i * 8)) & 0xFF);
}

short buffer::read_short(void)
{
	// 循环以BYTE位的方式填充短整形
	short value = _buffer[_start_offset++];
	for (size_t i = 1; i < sizeof(short); i++) {
		value = value << 8;
		value |= _buffer[_start_offset++];
	}

	return value;
}

void buffer::write_short(short value)
{
	ensure(sizeof(short));

	// 循环以BYTE位的方式写入缓存
	for (int i = sizeof(short)-1; i >= 0; i--)
		_buffer[_valid_offset++] = ((value >> (i * 8)) & 0xFF);
}

void buffer::read_data(char *data, int len)
{
	// 读取指定长度数据
	memcpy(data, &_buffer[_start_offset], len);
	_start_offset += len;
}

void buffer::write_data(char *data, int len)
{
	// 数据拷贝
	ensure(len);
	memcpy(&_buffer[_valid_offset], data, len);
}

