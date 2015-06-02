/*
 * logger.h
 *
 *  Created on: 2015年5月7日
 *      Author: root
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <mutex>
#include <thread>
#include <fstream>
#include <sstream>
#include <iostream>

#include "ebase.h"
#include "utils.h"

class logger {
public:
	logger();
	virtual ~logger();
	SINGLETON(logger)

	// 日志级别
	enum LOG_LEVEL {
		TRACE,
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL,
	};

	void print(std::string str);

	bool init(const char *file);
	bool init(const char *address, const int port);

	int get_type(void)			{ return _type; }
	int get_level(void)			{ return _level; }
	void set_type(int type)		{ _type = type; }
	void set_level(int level)	{ _level = level; }

private:
	int _type;		// 日志类型
	int _level;		// 日志级别
	std::mutex _mutex;	// 输出加锁
	std::ofstream _file;	// 日志文件
};

class log {
public:
	log(const char *file, const int line)
	: _line(line)
	, _file(utils::format_upper_file(file))
	{

	}

	virtual ~log()
	{
		// 格式化日志
		std::ostringstream oss;
		oss << utils::format_now_time() << " " << std::this_thread::get_id()
		<< " " << _stream.str() << " " << _file << ":" << _line;
		logger::ins().print(oss.str());
	}

	std::ostringstream &get_stream(void) { return _stream; }

private:
	int _line;	// 日志所在行
	std::string _file;	// 日志所在文件
	std::ostringstream _stream;	// 日志输出信息
};

#endif /* LOGGER_H_ */
