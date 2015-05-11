/*
 * logger.h
 *
 *  Created on: 2015年5月7日
 *      Author: root
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <time.h>
#include <ctype.h>
#include <string.h>

#include <thread>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

#include "logmgr.h"

class logger {
public:
	logger(const char *file, const int line);
	virtual ~logger();

	string format_time(void);
	string format_file(const char *file);
	ostringstream &get_stream(void)	{ return _stream; }

private:
	int _line;	// 日志所在行
	string _file;	// 日志所在文件
	ostringstream _stream;	// 日志输出信息
};

#endif /* LOGGER_H_ */
