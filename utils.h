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

#include <iostream>

class utils {
public:
	utils();
	virtual ~utils();

	static std::string format_now_time(void);

	static std::string format_upper_file(const char *file);
};

#endif /* UTILS_H_ */
