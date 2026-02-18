#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#include "../include/logger.h"

static logger_t logger = {
  .min_level = LOG_INFO,
  .log_file = NULL,
  .console_output = 1,
  .file_output = 0
};

void logger_init (log_level_t min_level, const char *log_file_path) {
  logger.min_level = min_level;

  // Open log file if path provided
  if (log_file_path != NULL) {
    logger.log_file = fopen(log_file_path, "a");  // Append mode
    if (logger.log_file == NULL) {
      fprintf(stderr, "Failed to open log file: %s\n", log_file_path);
      logger.file_output = 0;
    } else {
      logger.file_output = 1;

      // wWrite separator
      fprintf(logger.log_file,
              "\n===============================\n");
      fprintf(logger.log_file,
              "Server started at: %s",
              ctime(&(time_t){time(NULL)}));
      fprintf(logger.log_file,
              "==============================\n\n");
      fflush(logger.log_file);
    }
  }
}

void logger_set_console(int enabled) {
  logger.console_output = enabled;
}

void logger_set_file(int enabled) {
  logger.file_output = enabled;
}

void log_message (log_level_t level, const char *format, ...) {
  if (level < logger.min_level) {
    return;
  }

  // Get current time
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  // Level strings
  const char *level_str[] = {"DEBUG", "INFO", "WARN", "ERROR"};

  // Format timestamp
  char timestamp[32];
  snprintf(timestamp, sizeof(timestamp),
           "[%04d-%02d-%02d %02d:%02d:%02d]",
           t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
           t->tm_hour, t->tm_min, t->tm_sec);

  // Build the log message
  char message[1024];
  va_list args;
  va_start(args, format);
  vsnprintf(message, sizeof(message), format, args);
  va_end(args);

  // Output to console 
  if (logger.console_output) {
    printf("%s [%s] %s\n", timestamp, level_str[level], message);
  }

  // Output to file
  if (logger.file_output && logger.log_file != NULL) {
    fprintf(logger.log_file, "%s [%s] %s\n",
            timestamp, level_str[level], message);
    fflush(logger.log_file);  // Ensure it's written immediately
  }
}

void logger_close() {
  if (logger.log_file != NULL) {
    fprintf(logger.log_file,
            "\n==============================\n");
    fprintf(logger.log_file, "Server stopped\n");
    fprintf(logger.log_file,
            "==============================\n\n");
    fclose(logger.log_file);
    logger.log_file = NULL;
  }
}
