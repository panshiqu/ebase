/*
 * buffer.h
 *
 *  Created on: 2015年5月2日
 *      Author: root
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdlib.h>
#include <string.h>

#include "ebase.h"

class buffer {
public:
	buffer();
	virtual ~buffer();

public:
	int get_valid_offset(void)		{ return _valid_offset; }
	int get_idle_length(void)		{ return _total_length-_valid_offset; }
	int get_used_length(void)		{ return _valid_offset-_start_offset; }

	unsigned char *get_start_buffer(void)	{ return &_buffer[_start_offset]; }
	unsigned char *get_valid_buffer(void)	{ return &_buffer[_valid_offset]; }

	void incr_start_offset(int len)			{ _start_offset += len; }
	void decr_start_offset(int len)			{ _start_offset -= len; }
	void incr_valid_offset(int len)			{ _valid_offset += len; extra(); }

	void extra(void);
	void reset(void);
	bool check(void);
	void ensure(int len);

	int read_int(void);
	void write_int(int value);

	short read_short(void);
	void write_short(short value);

	void read_data(char *data, int len);
	void write_data(char *data, int len);

private:
	int _start_offset;	// 起始偏移
	int _valid_offset;	// 有效偏移
	int _total_length;	// 总长度
	unsigned char *_buffer;	// 缓存
};

#endif /* BUFFER_H_ */
