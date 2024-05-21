// Logger.cpp
#include "Logger.h"
#include <stdarg.h>
#include <stdio.h>


namespace Logger {
  namespace {
    int runtimeLogLevel = LOG_LEVEL;
  }
  void logPrint(int level,
    const char * file, int line,
      const char * format, ...) {
    auto time_ms = millis();
    if (level < runtimeLogLevel)
      return;

    const char * colorPrefix = "";
    const char * colorSuffix = "\033[0m"; // Reset to default terminal color
    const char * levelStr = "";
    switch (level) {
    case LOG_LEVEL_TRACE:
      colorPrefix = LOG_COLOR_TRACE;
      levelStr = "T";
      break;
    case LOG_LEVEL_DEBUG:
      colorPrefix = LOG_COLOR_DEBUG;
      levelStr = "D";
      break;
    case LOG_LEVEL_INFO:
      colorPrefix = LOG_COLOR_INFO;
      levelStr = "I";
      break;
    case LOG_LEVEL_WARN:
      colorPrefix = LOG_COLOR_WARN;
      levelStr = "W";
      break;
    case LOG_LEVEL_ERROR:
      colorPrefix = LOG_COLOR_ERROR;
      levelStr = "E";
      break;
    case LOG_LEVEL_CRITICAL:
      colorPrefix = LOG_COLOR_CRITICAL;
      levelStr = "C";
      break;
    }

    char msg[256]; // Increase size if needed
    va_list args;
    va_start(args, format);
    vsnprintf(msg, sizeof(msg), format, args);
    va_end(args);

    // Print the log level character with color, but keep the rest of the line in default color
    Serial.printf("[%lu] [%s%s%s] [%s:%d] %s\n", time_ms, colorPrefix, levelStr, colorSuffix, file, line, msg);
  }
  void setRuntimeLogLevel(int level) {
    runtimeLogLevel = level;
  }
}