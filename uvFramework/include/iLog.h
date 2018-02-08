#ifndef _DAS_ILOG_H_
#define _DAS_ILOG_H_

/**
* ©¥©¥©¥©¥©¥ÉñÊÞ³öÃ»©¥©¥©¥©¥©¥©¥
*¡¡¡¡¡¡¡¡©³©·¡¡    ©³©·
*¡¡¡¡¡¡©³©¿©ß©¥©¥©¥©¿©ß©·
*¡¡¡¡¡¡©§¡¡¡¡¡¡¡¡¡¡¡¡  ©§ ¡¡
*¡¡¡¡¡¡©§      ©¥¡¡¡¡  ©§
*¡¡¡¡¡¡¨€¨€¨€¨€©¥¨€¨€¨€¨€
*¡¡¡¡¡¡©§¡¡¡¡¡¡¡¡¡¡¡¡¡¡©§
*¡¡¡¡¡¡©§¡¡¡¡¡¡©ß¡¡¡¡¡¡©§
*¡¡¡¡¡¡©§¡¡¡¡¡¡¡¡	   ©§
*¡¡¡¡¡¡©»©¥©·¡¡¡¡¡¡©³©¥©¿
*¡¡¡¡¡¡¡¡¡¡©§¡¡¡¡¡¡©§¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡
*¡¡¡¡¡¡¡¡¡¡©§¡¡¡¡¡¡©§¡¡¡¡
*¡¡¡¡¡¡¡¡¡¡©§¡¡¡¡¡¡©§
*¡¡¡¡¡¡¡¡¡¡©§¡¡¡¡¡¡©§¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡
*¡¡¡¡¡¡¡¡¡¡©§¡¡ ¡¡ ©»©¥©¥©¥©·
*¡¡¡¡¡¡¡¡¡¡©§ ¡¡¡¡¡¡¡¡     ©Ç©·
*¡¡¡¡¡¡¡¡¡¡©§ ¡¡¡¡¡¡¡¡¡¡   ©³©¿
*¡¡¡¡¡¡¡¡¡¡©»©·©·©³©¥©×©·©³©¿
*¡¡¡¡¡¡¡¡¡¡¡¡©§©Ï©Ï  ©§©Ï©Ï
*¡¡¡¡¡¡¡¡¡¡¡¡©»©ß©¿  ©»©ß©¿
*
* Create by chen, 2017/12/25
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#if ( defined _WIN32 )
#include <windows.h>
#include <share.h>
#include <io.h>
#include <fcntl.h>
#elif ( defined __unix ) || ( defined _AIX ) || ( defined __linux__ ) || ( defined __hpux )
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <syslog.h>
#include <pthread.h>
#include <sys/syscall.h>
#endif

#if ( defined _WIN32 )
#define LOG_NEWLINE		"\r\n"
#define LOG_NEWLINE_LEN		2
#ifndef snprintf
#define snprintf		_snprintf
#endif
#define igetpid			GetCurrentProcessId
#define pthread_self()	GetCurrentThreadId()
#elif ( defined __unix ) || ( defined _AIX ) || ( defined __linux__ ) || ( defined __hpux )
#define LOG_NEWLINE		"\n"
#define LOG_NEWLINE_LEN		1
#define pthread_self()	(syscall(SYS_gettid))
#endif

typedef enum log_save_type{
	ILOG_SAVE_UNKNOW = 0,
	ILOG_SAVE_FILE,
	ILOG_SAVE_CALLBACK,
}log_save_type_e;

typedef enum log_print_type {
	ILOG_TYPE_FATAL,
	ILOG_TYPE_ERROR,
	ILOG_TYPE_WARN,
	ILOG_TYPE_INFO,
	ILOG_TYPE_DEBUG
}log_print_type_e;


#define DAFAULT_LINE_SIZE	64
#define LOG_MAX_FILE_PATH_LENGTH 1024
#define MAX_LINE_SIZE LOG_MAX_FILE_PATH_LENGTH
#define DEFAULT_LOG_PRINT_LEVEL	ILOG_TYPE_INFO
#define DEFAULT_LOG_SAVE_TYPE	ILOG_SAVE_FILE


typedef void(*logCallback)(int, const char*, va_list);

typedef struct log_handle{

	log_save_type_e		save_type;
	log_print_type_e	print_type;
	char				log_path[LOG_MAX_FILE_PATH_LENGTH];

	FILE*				fd;
	logCallback			callback;

}log_handle_t;

//////////////////////////////////////////////////////////////////////////////////////

log_handle_t* LogInit();

void LogUninit();

void LogThreshold(log_print_type_e level);

void LogSetCallback(void(*callback)(int, const char*, va_list));

void LogSetPath(const char* filename);

void LogDebug(char *filename, long fileline, const char* format, ...);

void LogInfo(char *filename, long fileline, const char* format, ...);

void LogWarn(char *filename, long fileline, const char* format, ...);

void LogError(char *filename, long fileline, const char* format, ...);

void LogFatal(char *filename, long fileline, const char* format, ...);

/////////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define iLogDebug( ... )		LogDebug( __FILENAME__ , __LINE__ , ##__VA_ARGS__ );
#define iLogInfo( ... )			LogInfo( __FILENAME__ , __LINE__ , ##__VA_ARGS__ );
#define iLogWarn( ... )			LogWarn( __FILENAME__ , __LINE__ , ##__VA_ARGS__ );
#define iLogError( ... )		LogError( __FILENAME__ , __LINE__ , ##__VA_ARGS__ );
#define iLogFata( ... )		    LogFatal( __FILENAME__ , __LINE__ , ##__VA_ARGS__ );

#endif
