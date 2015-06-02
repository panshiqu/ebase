/*
 * utils.cpp
 *
 *  Created on: 2015年6月1日
 *      Author: root
 */

#include "utils.h"

utils::utils()
{

}

utils::~utils()
{

}

string utils::format_now_time(void)
{
	// 获取时间
	char sztime[20];
	time_t curtime = time(NULL);
	memset(sztime, 0, sizeof(sztime));
	struct tm *t = localtime(&curtime);
	strftime(sztime, sizeof(sztime), "%Y-%m-%d %H:%M:%S", t);

	return sztime;
}

string utils::format_upper_file(const char *file)
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

