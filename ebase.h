/*
 * ebase.h
 *
 *  Created on: 2015年5月6日
 *      Author: root
 */

#ifndef EBASE_H_
#define EBASE_H_

#define NR_EVENT		10
#define NR_THREAD		16
#define NR_TIMEOUT	1000
#define NR_BUFFER		1024

enum LOG_TYPE
{
	LOG_NET				= 0,	// 网络日志
	LOG_FILE				= 1,	// 文件日志
	LOG_CONSOLE			= 2,	// 控制台日志
};

// 单例模式宏
#define SINGLETON(classname) \
	static classname &ins(void) \
	{ \
		static classname instance; \
		return instance; \
	}

#endif /* EBASE_H_ */
