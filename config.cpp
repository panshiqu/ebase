/*
 * config.cpp
 *
 *  Created on: 2015年6月4日
 *      Author: root
 */

#include "config.h"

using namespace std;

config::config()
{

}

config::~config()
{
	_configs.clear();
}

void config::trim(char *str)
{
	char *pre = (char *)str;
	char *post = (char *)str;

	while (*post) {
		if (*post == ' ' || *post == ',' ||
			*post == '"' || *post == '\n' ||
			*post == '\t') {
			post++;
			continue;
		}

		*pre = *post;

		pre++;
		post++;
	}

	*pre = '\0';
}

bool config::load(const char *file)
{
	// 打开文件
	FILE *fd = fopen(file, "rb");
	if (fd == NULL) {
		LOG_ERROR << "fopen error.";
		return false;
	}

	char buf[NR_BUFFER] = {'\0'};
	while (!feof(fd)) {
		// 数据清空
		memset(buf, '\0', NR_BUFFER);

		// 读取一行
		if (fgets(buf, NR_BUFFER, fd) == NULL) break;

		// 整理数据
		this->trim(buf);

		// 仅仅是换行
		if (buf[0] == '\0') continue;

		// 是否是注释
		if (buf[0] == '/' && buf[1] == '/') continue;

		// 读取数据
		string key = strtok(buf, "=");
		string data = strtok(NULL, "=");

		// 更新MAP
		_configs.insert(make_pair(key, data));
	}

	fclose(fd);
	return true;
}

int config::get_int(std::string name)
{
	map<string, string>::iterator itr = _configs.find(name);
	if (itr == _configs.end()) return 0;
	return atoi(itr->second.c_str());
}

std::string config::get_string(std::string name)
{
	map<string, string>::iterator itr = _configs.find(name);
	if (itr == _configs.end()) return "";
	return itr->second.c_str();
}

