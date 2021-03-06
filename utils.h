/*
 * utils.h
 *
 *  Created on: 2015年6月1日
 *      Author: root
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <time.h>		// for time
#include <ctype.h>	// for toupper
#include <string.h>	// for memset

#include <map>
#include <list>
#include <iostream>

class utils {
public:
	utils();
	virtual ~utils();

	static std::string format_now_time(void);

	static std::string format_upper_file(const char *file);

	static time_t str2time(std::string str);

	static std::string time2str(time_t tt);

	static void split_str(std::list<int> &splits, std::string source, std::string delimiter);

	static void split_str(std::list<std::string> &splits, std::string source, std::string delimiter);

	static void split_str(std::map<std::string, std::string> &splits, std::string source, std::string delimiter1, std::string delimiter2);
};

#endif /* UTILS_H_ */
