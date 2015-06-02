/*
 * utils.h
 *
 *  Created on: 2015年6月1日
 *      Author: root
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <time.h>
#include <ctype.h>
#include <string.h>

#include <iostream>
using namespace std;

class utils {
public:
	utils();
	virtual ~utils();

	static string format_now_time(void);

	static string format_upper_file(const char *file);
};

#endif /* UTILS_H_ */
