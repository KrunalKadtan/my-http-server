// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "../include/socket.h"
#include "../include/logger.h"

// Global server socket for signal handler
int server_fd_global = -1;

// Signal handler function
void handle_sigint(int sig) {

  log_message(LOG_INFO, "Received %d SIGINT (Ctrl+C)", sig);
  log_message(LOG_INFO, "Shutting down gracefully...");

  // Close the server socket
  if (server_fd_global >= 0) {
    close(server_fd_global);
    log_message(LOG_INFO, "Server socket closed");
  }

  logger_close();
  exit(0);
}

int main()
{

  // Initialize logger with file output
  logger_init(LOG_DEBUG, "server.log");

  log_message(LOG_INFO, "==============================");
  log_message(LOG_INFO, "Stating HTTP server...");
  log_message(LOG_INFO, "==============================");

  // Register signal handler
  signal(SIGINT, handle_sigint);
  log_message(LOG_DEBUG, "Signal handler registered");

  // Create server socket
  int server_fd = create_server_socket(8080);
  if (server_fd < 0)
  {
    log_message(LOG_ERROR, "Failed to create server socket");
    logger_close();
    exit(EXIT_FAILURE);
  }

  // Store in global for signal handler
  server_fd_global = server_fd;

  log_message(LOG_INFO, "Server listening on http://localhost:8080");
  log_message(LOG_INFO, "Logs are being written to server.log");
  log_message(LOG_INFO, "Press Ctrl+C to stop\n");

  // Accept Loop
  int request_count = 0;
  while (1)
  {
    // Accept a connection
    int client_fd = accept_client(server_fd);
    if (client_fd < 0)
    {
      log_message(LOG_WARN, "Failed to accept client");
      continue; // Skip this iteration, try again
    }

    request_count++;
    log_message(LOG_DEBUG, "Client connected (fd: %d, request #%d)", client_fd, request_count);

    // Read request
    char buffer[4096] = {0}; // Initialize all bytes to zero
    // Read data from client (blocks until data arrives)
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);

    if (bytes_read <= 0)
    {
      log_message(LOG_WARN, "Failed to read from client");
      close(client_fd);
      continue;
    }
      // Parse request
      char method[16], path[256], version[16];

      if (sscanf(buffer, "%s %s %s", method, path, version) == 3) {
        log_message(LOG_INFO, "%s %s %s", method, path, version);

        // Build response
        char *body = "Hello from modular HTTP server!";
        char response[2048];
      snprintf(response, sizeof(response),
              "HTTP/1.1 200 OK\r\n"
              "Content-Type: text/plain\r\n"
              "Content-Length: %zu\r\n"
              "\r\n"
               "%s",
              strlen(body), body
               );

      // Send response
      ssize_t bytes_written = write(client_fd, response, strlen(response));
      if (bytes_written > 0) {
        log_message(LOG_DEBUG, "Sent %zd bytes to client", bytes_written);
      } else {
        log_message(LOG_ERROR, "Failed to send response");
      }
      } else {
      log_message(LOG_WARN, "Malformed request");
    }

    // Close the connection
    close(client_fd);
    log_message(LOG_DEBUG, "Client connection closed");
  }

  // Close the connection
  close(server_fd);
  logger_close();

  return 0;
}
