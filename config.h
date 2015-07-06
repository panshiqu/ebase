/*
 * config.h
 *
 *  Created on: 2015年6月4日
 *      Author: root
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <map>
#include <iostream>

#include "logger.h"
#include "ebase.h"

class config {
public:
	config();
	virtual ~config();

	void trim(char *str);
	virtual bool is_trim(const char ch);
	virtual bool load(const char *file);

	int get_int(std::string name);
	std::string get_string(std::string name);

protected:	// 子类也能直接访问（纵然不好）
	std::map<std::string, std::string> _configs;
};

#endif /* CONFIG_H_ */
