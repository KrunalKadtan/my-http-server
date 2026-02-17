#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR
} log_level_t;

// Initialize logger 
void logger_init (log_level_t min_level);

// Log a message
void log_message (log_level_t level, const char *format, ...);

#endif  // LOGGER_H
