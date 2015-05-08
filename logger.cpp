/*
 * logger.cpp
 *
 *  Created on: 2015年5月7日
 *      Author: root
 */

#include "logger.h"

logger::logger(const char *file, int line)
{
	cout << "hello" << endl;
}

logger::~logger()
{
	cout << "~hello" << endl;
}

string logger::format_time(void)
{
	// 获取时间
	char sztime[20];
	time_t curtime = time(NULL);
	memset(sztime, 0, sizeof(sztime));
	struct tm *t = localtime(&curtime);
	strftime(sztime, sizeof(sztime), "%Y-%m-%d %H:%M:%S", t);

	return sztime;
}

string logger::format_file(const char *file)
{
	string str(file);

	// 取文件名
	size_t pos = str.find_last_of("/");
	if (pos != string::npos) str = str.substr(pos+1);

	// 全部转换成大写
	for (size_t i = 0; i < str.size(); i++)
		str[i] = toupper(str[i]);

	return str;
}

//ostringstream &logger::get_stream(void)
//{
//	// 创建格式化输出流
//	ostringstream *ostr = new ostringstream();
//
//	// 更新LIST（加锁）
//	unique_lock<mutex> ulock(_mutex);
//	_streams.push_back(ostr);
//	return *ostr;
//}

void logger::print(void)
{
//	for (auto s : _streams)
//		cout << s->str() << endl;
}

