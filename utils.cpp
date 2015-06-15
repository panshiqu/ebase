/*
 * utils.cpp
 *
 *  Created on: 2015年6月1日
 *      Author: root
 */

#include "utils.h"

using namespace std;

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

time_t utils::str2time(std::string str)
{
	// 字符串转时间
	struct tm t = {0};
	strptime(str.c_str(), "%Y-%m-%d %H:%M:%S", &t);

	return mktime(&t);
}

std::string utils::time2str(time_t tt)
{
	// 时间转字符串
	char str[20] = {0};
	struct tm *t = localtime(&tt);
	strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", t);

	return str;
}

void utils::split_str(std::list<int> &splits, std::string source, std::string delimiter)
{
	// 切分字符串
	size_t index;
	while ((index = source.find(delimiter)) != string::npos) {
		splits.push_back(atoi(source.substr(0, index).c_str()));
		source = source.substr(index+1, -1);
	}

	if (!source.empty())
		splits.push_back(atoi(source.c_str()));
}

void utils::split_str(std::list<std::string> &splits, std::string source, std::string delimiter)
{
	// 切分字符串
	size_t index;
	while ((index = source.find(delimiter)) != string::npos) {
		splits.push_back(source.substr(0, index).c_str());
		source = source.substr(index+1, -1);
	}

	if (!source.empty())
		splits.push_back(source);
}

