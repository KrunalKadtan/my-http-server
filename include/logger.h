#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

typedef enum {
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR
} log_level_t;

typedef struct {
  log_level_t min_level;
  FILE *log_file;
  int console_output;
  int file_output;
} logger_t;

// Initialize logger with options
void logger_init (log_level_t min_level, const char *log_file_path);

// Set whether to output to console 
void logger_set_console(int enabled);

// Set whether to output to file 
void logger_set_file(int enabled);

// Log a message
void log_message (log_level_t level, const char *format, ...);

// Close logger and cleanup
void logger_close();

#endif  // LOGGER_H
