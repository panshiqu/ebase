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

enum LOG_TYPE {
	LOG_CONSOLE			= 1,	// 控制台日志
	LOG_FILE				= 2,	// 文件日志
	LOG_NET				= 4,	// 网络日志
};

#define LOG_TRACE if (logmgr::ins().get_level() <= logmgr::LOG_LEVEL::TRACE) \
	logger(__FILE__, __LINE__).get_stream() << "TRACE: "
#define LOG_DEBUG if (logmgr::ins().get_level() <= logmgr::LOG_LEVEL::DEBUG) \
	logger(__FILE__, __LINE__).get_stream() << "DEBUG: "
#define LOG_INFO if (logmgr::ins().get_level() <= logmgr::LOG_LEVEL::INFO) \
	logger(__FILE__, __LINE__).get_stream() << "INFO: "
#define LOG_WARN if (logmgr::ins().get_level() <= logmgr::LOG_LEVEL::WARN) \
	logger(__FILE__, __LINE__).get_stream() << "WARN: "
#define LOG_ERROR if (logmgr::ins().get_level() <= logmgr::LOG_LEVEL::ERROR) \
	logger(__FILE__, __LINE__).get_stream() << "ERROR: "
#define LOG_FATAL if (logmgr::ins().get_level() <= logmgr::LOG_LEVEL::FATAL) \
	logger(__FILE__, __LINE__).get_stream() << "FATAL: "

// 单例模式宏
#define SINGLETON(classname) \
	static classname &ins(void) \
	{ \
		static classname instance; \
		return instance; \
	}

#endif /* EBASE_H_ */
