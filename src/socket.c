#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

#include "../include/socket.h"
#include "../include/logger.h"

int create_server_socket(int port) {
  
  // Create socket
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("socket creation failed");
    return -1;
  }

  // Set SO_REUSEADDR
  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt failed");
    close(server_fd);
    return -1;
  }

  // Prepare address
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  // Bind
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    close(server_fd);
    return -1;
  }

  // Listen
  if (listen(server_fd, 10) < 0) {
    perror("listen failed");
    close(server_fd);
    return -1;
  }

  return server_fd;
}

int accept_client(int server_fd) {
  int client_fd = accept(server_fd, NULL, NULL);

  if (client_fd < 0) {
    // Check if interrupted by signal
    if (errno == EINTR) {
      log_message(LOG_INFO, "Accept interrupted by signal");
    }
    perror("accept failed");
    return -1;
  }

  return client_fd;
}
