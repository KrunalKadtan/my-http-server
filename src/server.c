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

  log_message(LOG_INFO, "Caught %d SIGINT, shutting down gracefully", sig);

  // Close the server socket
  if (server_fd_global >= 0) {
    close(server_fd_global);
  }

  exit(0);
}

int main()
{

  // Initialize logger
  logger_init(LOG_DEBUG);
  log_message(LOG_INFO, "Stating HTTP server...");

  // Register signal handler
  signal(SIGINT, handle_sigint);

  // Create server socket
  int server_fd = create_server_socket(8080);
  if (server_fd < 0)
  {
    log_message(LOG_ERROR, "Failed to create server socket");
    exit(EXIT_FAILURE);
  }

  // Store in global for signal handler
  server_fd_global = server_fd;

  log_message(LOG_INFO, "Server listening on http://localhost:8080");
  log_message(LOG_INFO, "Press Ctrl+C to stop\n");

  // Accept Loop
  while (1)
  {
    // Accept a connection
    int client_fd = accept_client(server_fd);
    if (client_fd < 0)
    {
      continue; // Skip this iteration, try again
    }

    log_message(LOG_DEBUG, "Client connected (fd: %d)", client_fd);

    // Read request
    char buffer[4096] = {0}; // Initialize all bytes to zero
    // Read data from client (blocks until data arrives)
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);

    if (bytes_read < 0)
    {
      log_message(LOG_ERROR, "Read failed");
      close(client_fd);
      continue;
    }
    else if (bytes_read == 0)
    {
      log_message(LOG_INFO, "Client disconnected");
      close(client_fd);
      continue;
    }
    else
    {
      // Parse the request line 
      char method[16] = {0};
      char path[256] = {0};
      char version[16] = {0};

      // Extract method, path & version from the first line
      
      if (sscanf(buffer, "%s %s %s", method, path, version) == 3) {
        log_message(LOG_INFO, "Request: %s %s", method, path);

        // Send response
        char *response =
              "HTTP/1.1 200 OK\r\n"
              "Content-Type: text/plain\r\n"
              "Content-Length: 13\r\n"
              "\r\n"
              "Hello, World";

        write(client_fd, response, strlen(response));
        log_message(LOG_DEBUG, "Response sent");
      }
    }

    // Close the connection
    close(client_fd);
  }

  // Close the connection
  close(server_fd);

  return 0;
}
