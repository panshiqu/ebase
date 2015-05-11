/*
 * logmgr.cpp
 *
 *  Created on: 2015年5月10日
 *      Author: root
 */

#include "logmgr.h"

logmgr::logmgr()
	: _type(LOG_CONSOLE)
	, _level(LOG_LEVEL::TRACE)
{

}

logmgr::~logmgr()
{
	// 关闭文件
	_file.close();
}

bool logmgr::init(const char *file)
{
	// 打开文件
	_file.open(file, ios_base::app);
	return true;
}

bool logmgr::init(const char *address, const int port)
{
	// 暂未实现
	return true;
}

void logmgr::print(string str)
{
	// 不管输出到那里加锁先
	unique_lock<mutex> ulock(_mutex);

	// 输出至控制台
	if (_type & LOG_CONSOLE)
		cout << str << endl;

	// 输出至文件
	if (_type & LOG_FILE)
		_file << str << endl;

	// 输出至网络
	if (_type & LOG_NET)
		cout << "unimplemented" << endl;
}

