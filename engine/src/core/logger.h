#pragma once

#include "defines.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

// disable debug and trace logging for release builds...
#if KRELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

typedef enum log_level {
    LOG_LEVLEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5,
} log_level;

b8 initialize_logging();
void shutdown_logging();

KAPI void log_output(log_level level, const char* msg, ...);

// logs a fatal level msg
#define KFATAL(msg, ...) log_output(LOG_LEVLEL_FATAL, msg, ##__VA_ARGS__);

// logs an error level msg
#ifndef KERROR
#define KERROR(msg, ...) log_output(LOG_LEVEL_ERROR, msg, ##__VA_ARGS__);
#endif

// logs a warn level msg
#if LOG_WARN_ENABLED == 1
#define KWARN(msg, ...) log_output(LOG_LEVEL_WARN, msg, ##__VA_ARGS__);
#else
#define KWARN(msg, ...)
#endif

// logs a info level msg
#if LOG_INFO_ENABLED == 1
#define KINFO(msg, ...) log_output(LOG_LEVEL_INFO, msg, ##__VA_ARGS__);
#else
#define KINFO(msg, ...)
#endif

// logs a debug level msg
#if LOG_DEBUG_ENABLED == 1
#define KDEBUG(msg, ...) log_output(LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__);
#else
#define KDEBUG(msg, ...)
#endif

// logs a trace level msg
#if LOG_TRACE_ENABLED == 1
#define KTRACE(msg, ...) log_output(LOG_LEVEL_TRACE, msg, ##__VA_ARGS__);
#else
#define KTRACE(msg, ...)
#endif