#pragma once
#include <Arduino.h>

// Log levels
#define LOG_LEVEL_TRACE 0
#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_WARN 3
#define LOG_LEVEL_ERROR 4
#define LOG_LEVEL_CRITICAL 5

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_TRACE // Default compile-time log level
#endif

namespace Logger {
  void logPrint(int level,
    const char * file, int line,
      const char * format, ...);
  void setRuntimeLogLevel(int level);
}

#define LOG_COLOR_TRACE "\033[94m" // Blue
#define LOG_COLOR_DEBUG "\033[34m" // Darker Blue
#define LOG_COLOR_INFO "\033[32m" // Green
#define LOG_COLOR_WARN "\033[93m" // Yellow
#define LOG_COLOR_ERROR "\033[91m" // Red
#define LOG_COLOR_CRITICAL "\033[101m\033[93m" // Yellow on red background

// Compile-time filtering
#if LOG_LEVEL <= LOG_LEVEL_TRACE
#define LOG_TRACE(...) Logger::logPrint(LOG_LEVEL_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#else
#define LOG_TRACE(...)
#endif

#if LOG_LEVEL <= LOG_LEVEL_DEBUG
#define LOG_DEBUG(...) Logger::logPrint(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif

#if LOG_LEVEL <= LOG_LEVEL_INFO
#define LOG_INFO(...) Logger::logPrint(LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)
#else
#define LOG_INFO(...)
#endif

#if LOG_LEVEL <= LOG_LEVEL_WARN
#define LOG_WARN(...) Logger::logPrint(LOG_LEVEL_WARN, __FILE__, __LINE__, __VA_ARGS__)
#else
#define LOG_WARN(...)
#endif

#if LOG_LEVEL <= LOG_LEVEL_ERROR
#define LOG_ERROR(...) Logger::logPrint(LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#else
#define LOG_ERROR(...)
#endif

#if LOG_LEVEL <= LOG_LEVEL_CRITICAL
#define LOG_CRITICAL(...) Logger::logPrint(LOG_LEVEL_CRITICAL, __FILE__, __LINE__, __VA_ARGS__)
#else
#define LOG_CRITICAL(...)
#endif