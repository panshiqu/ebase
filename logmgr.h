/*
 * logmgr.h
 *
 *  Created on: 2015年5月10日
 *      Author: root
 */

#ifndef LOGMGR_H_
#define LOGMGR_H_

#include <mutex>
#include <fstream>
#include <iostream>
using namespace std;

#include "ebase.h"

class logmgr {
public:
	logmgr();
	virtual ~logmgr();
	SINGLETON(logmgr)

	enum LOG_LEVEL {
		TRACE,
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL,
	};

	void print(string str);

	bool init(const char *file);
	bool init(const char *address, const int port);

	int get_type(void)			{ return _type; }
	int get_level(void)			{ return _level; }
	void set_type(int type)		{ _type = type; }
	void set_level(int level)	{ _level = level; }

private:
	int _type;		// 日志类型
	int _level;		// 日志级别
	mutex _mutex;
	ofstream _file;
};

#endif /* LOGMGR_H_ */
