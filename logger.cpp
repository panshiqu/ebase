/*
 * logger.cpp
 *
 *  Created on: 2015年5月7日
 *      Author: root
 */

#include "logger.h"

logger::logger(const char *file, const int line)
	: _line(line)
	, _file(format_file(file))
{

}

logger::~logger()
{
	// 格式化日志
	ostringstream oss;
	oss << format_time() << " " << this_thread::get_id()
	<< " " << _stream.str() << " " << _file << ":" << _line;
	logmgr::ins().print(oss.str());
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

