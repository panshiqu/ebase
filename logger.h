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

#include <list>
#include <mutex>
#include <thread>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

#include "ebase.h"

#define LOG_ALL logger::ins().format_time() << " " << this_thread::get_id() \
	<< " " << __LINE__ << ":" << logger::ins().format_file(__FILE__)
#define LOG_TRACE if (logger::ins().get_level() <= logger::LOG_LEVEL::TRACE) \
	logger::ins().get_stream() << LOG_ALL << " TRACE : "
//#define LOG_DEBUG if (logger::ins().get_level() <= logger::LOG_LEVEL::DEBUG) \
//	logger::ins().get_stream() << LOG_ALL << " DEBUG : "
//#define LOG_INFO if (logger::ins().get_level() <= logger::LOG_LEVEL::INFO) \
//	logger::ins().get_stream() << LOG_ALL << " INFO : "
//#define LOG_WARN if (logger::ins().get_level() <= logger::LOG_LEVEL::WARN) \
//	logger::ins().get_stream() << LOG_ALL << " WARN : "
//#define LOG_ERROR if (logger::ins().get_level() <= logger::LOG_LEVEL::ERROR) \
//	logger::ins().get_stream() << LOG_ALL << " ERROR : "
//#define LOG_FATAL if (logger::ins().get_level() <= logger::LOG_LEVEL::FATAL) \
//	logger::ins().get_stream() << LOG_ALL << " FATAL : "

class logger {

public:
enum LOG_LEVEL {
	TRACE,
	DEBUG,
	INFO,
	WARN,
	ERROR,
	FATAL,
};

public:
	logger(const char *file, int line);
	virtual ~logger();

	bool init(void);
	bool init(const char *file);
	bool init(const char *address, const int port);


	int get_level(void)			{ return _level; }
	void set_level(int level)	{ _level = level; }
	ostringstream &get_stream(void)	{ return _stream; }

	string format_time(void);
	string format_file(const char *file);



	void print(void);

private:
	void __log(LOG_LEVEL level, ...);

private:
	int _level;
	mutex _mutex;
	thread _thread;
	ostringstream _stream;
};

#endif /* LOGGER_H_ */
