#include "iLog.h"
#include <iostream>

static log_handle_t	*g_log_handle = NULL;

static void pre_write_log(char *filename, long fileline, const log_print_type_e type, const char* format, const va_list varArgs);
static void write_log(const char* format, ...);
static const char* type_to_string(const log_print_type_e type);

log_handle_t* LogInit()
{
	g_log_handle = (log_handle_t*)malloc(sizeof(log_handle_t));
	memset(g_log_handle, 0, sizeof(log_handle_t));

	g_log_handle->fd = NULL;
	g_log_handle->callback = NULL;
	g_log_handle->print_type = DEFAULT_LOG_PRINT_LEVEL;
	g_log_handle->save_type = DEFAULT_LOG_SAVE_TYPE;

	return g_log_handle;
}

void LogUninit()
{
	if (g_log_handle) {
		if (g_log_handle->fd != NULL) {
			fclose(g_log_handle->fd);
			g_log_handle->fd = NULL;
		}

		free(g_log_handle);
		g_log_handle = NULL;
	}
}

void LogThreshold(log_print_type_e level)
{
	if (g_log_handle) {
		g_log_handle->print_type = level;
	}
}

void LogSetCallback(void(*callback)(int, const char*, va_list))
{
	if (g_log_handle) {
		g_log_handle->callback = callback;
	}
}

void LogSetPath(const char* filename)
{
	if (g_log_handle) {
		int length = strlen(filename);
		memcpy(g_log_handle->log_path, filename, length - 4);

		char time_buf[DAFAULT_LINE_SIZE] = { 0 };
		time_t timestamp;
		time(&timestamp);
		struct tm *local_time = localtime(&timestamp);
		snprintf(time_buf, sizeof(time_buf), "%4d_%02d_%02d_%02d_%02d_%02d", local_time->tm_year + 1900, local_time->tm_mon + 1,
			local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

		unsigned long pid = (long)igetpid();
		char filepath[MAX_LINE_SIZE + DAFAULT_LINE_SIZE] = { 0 };
		snprintf(filepath, sizeof(filepath), "%s_%lu_%s.log", g_log_handle->log_path, pid, time_buf);

		g_log_handle->fd = fopen(filepath, "wb+");
	}
}

void LogDebug(char *filename, long fileline, const char* format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	pre_write_log(filename, fileline, ILOG_TYPE_DEBUG, format, varArgs);
	va_end(varArgs);
}

void LogInfo(char *filename, long fileline, const char* format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	pre_write_log(filename, fileline, ILOG_TYPE_INFO, format, varArgs);
	va_end(varArgs);
}

void LogWarn(char *filename, long fileline, const char* format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	pre_write_log(filename, fileline, ILOG_TYPE_WARN, format, varArgs);
	va_end(varArgs);
}

void LogError(char *filename, long fileline, const char* format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	pre_write_log(filename, fileline, ILOG_TYPE_ERROR, format, varArgs);
	va_end(varArgs);
}

void LogFatal(char *filename, long fileline, const char* format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	pre_write_log(filename, fileline, ILOG_TYPE_FATAL, format, varArgs);
	va_end(varArgs);
}

static void pre_write_log(char *filename, long fileline, const log_print_type_e type, const char* format, const va_list varArgs)
{
	if (g_log_handle == NULL) {
		return;
	}
	if (type > g_log_handle->print_type) {
		return;
	}

	if (g_log_handle->callback != NULL) {
		g_log_handle->callback(type, format, varArgs);
		return;
	}

	char log_message[MAX_LINE_SIZE] = { 0 };
	vsnprintf(log_message, sizeof(log_message), format, varArgs);

	char time_buf[DAFAULT_LINE_SIZE] = { 0 };
#ifdef WIN32
	SYSTEMTIME win_time;
	GetLocalTime(&win_time);
	snprintf(time_buf, DAFAULT_LINE_SIZE, "[%02d:%02d:%02d.%03d]",
		win_time.wHour, win_time.wMinute, win_time.wSecond, win_time.wMilliseconds);
#else
	struct timeval tv;
	gettimeofday (&tv, NULL);
	struct tm *ptm = localtime (&tv.tv_sec);
	snprintf(time_buf, sizeof(time_buf), "[%02d:%02d:%02d.%03d]",ptm->tm_hour, ptm->tm_min, ptm->tm_sec, tv.tv_usec / 1000);
#endif

	char source_buf[DAFAULT_LINE_SIZE] = { 0 };
	snprintf(source_buf, sizeof(source_buf), "[%s:%ld]", filename, fileline);

	unsigned long tid = pthread_self();

	write_log("%s%s[%lu]%s%s%s", time_buf, type_to_string(type), tid, source_buf, log_message, LOG_NEWLINE);
}

void write_log(const char* format, ...)
{
	char buffer[MAX_LINE_SIZE + DAFAULT_LINE_SIZE * 2] = {0};

	va_list varArgs;
	va_start(varArgs, format);
	vsnprintf(buffer, sizeof(buffer), format, varArgs);
	va_end(varArgs);

	int length = strlen(buffer);
	if (g_log_handle) {
		if (g_log_handle->fd != NULL) {
			fwrite(buffer, length, 1, g_log_handle->fd);
			fflush(g_log_handle->fd);
		}
		std::cout << buffer << std::endl;
	}
}

const char* type_to_string(const log_print_type_e type) {
	switch (type) {
	case ILOG_TYPE_FATAL:
		return "[FATAL]";
	case ILOG_TYPE_ERROR:
		return "[ERROR]";
	case ILOG_TYPE_WARN:
		return "[WARN]";
	case ILOG_TYPE_INFO:
		return "[INFO]";
	case ILOG_TYPE_DEBUG:
		return "[DEBUG]";
	default:
		break;
	}
	return "[UNKWN]";
}
