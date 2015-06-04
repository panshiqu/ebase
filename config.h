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
	SINGLETON(config)

	void trim(char *str);
	bool load(const char *file);

	int get_int(std::string name);
	std::string get_string(std::string name);

private:
	std::map<std::string, std::string> _configs;
};

#endif /* CONFIG_H_ */
