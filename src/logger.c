#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "../include/logger.h"

static log_level_t min_log_level = LOG_INFO;

void logger_init (log_level_t min_level) {
  min_log_level = min_level;
}

void log_message (log_level_t level, const char *format, ...) {
  if (level < min_log_level) {
    return;
  }

  // Get current time
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  // Level strings
  const char *level_str[] = {"DEBUG", "INFO", "WARN", "ERROR"};

  // Print timestamp and level
  printf("[%02d:%02d:%02d] [%s]",
         t->tm_hour, t->tm_min, t->tm_sec, level_str[level]);

  // Print the actual message using variable arguments
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);

  printf("\n");
}
