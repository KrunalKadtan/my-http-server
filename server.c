// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
  // Create a socket
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Set sokcet options to reuse address (helpful during development)
  int opt = 1;
  if (setsocketopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsocketopt failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Prepare the address structure
  struct sockaddr_in address;
  address.sin_family = AF_INET;         // IPv4
  address.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces (0.0.0.0)
  address.sin_port = htons(8080);       // Port 8080 (host to network byte order)

  // Bind the socket to the address
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Listen for connections
  if (listen(server_fd, 1) < 0)
  {
    perror("listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Server listening on http://localhost:8080\n");
  printf("Press Ctrl+C to stop\n\n");

  while (1)
  {
    printf("Waiting for next client...\n");

    // Accept a connection
    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0)
    {
      perror("accept failed");
      continue; // Skip this iteration, try again
    }

    printf("Client connected!\n");

    // Buffer to hold incoming data
    char buffer[4096] = {0}; // Initialize all bytes to zero

    // Read data from client (blocks until data arrives)
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);

    if (bytes_read < 0)
    {
      perror("Read Failed...!");
      close(client_fd);
      continue;
    }
    else if (bytes_read == 0)
    {
      printf("Client Disconnected.\n");
      close(client_fd);
      continue;
    }
    else
    {
      // bytes_read > 0 : We received data
      printf("\nReceived %zd bytes from client.\n", bytes_read);
      printf("\nClient sent:\n%s\n", buffer);

      // Parse the request line 
      char method[16] = {0};
      char path[256] = {0};
      char version[16] = {0};

      // Extract method, path & version from the first line
      int parsed = sscanf(buffer, "%s %s %s", method, path, version);

      
      if (parsed == 3) {
        printf("Method: %s\n", method);
        printf("Path: %s\n", path);
        printf("Version: %s\n", version);
        
        // Route based on path 
        char *body;
        
        
        if (strcmp(path, "/") == 0) {
          body = "Home Page";
        } else if (strcmp(path, "/about") == 0) {
          body = "About Page";
        } else {
          body = "404 Not Found";
        }

        // Build HTTP response
        char http_response[2048];
      
        int body_length = strlen(body);

        // Format the complete HTTP response
        sprintf(http_response,
              "HTTP/1.1 200 OK\r\n"
              "Content-Type: text/plain\r\n"
              "Content-Length: %d\r\n"
              "\r\n"
              "%s",
              body_length,
              body);

        // Send the HTTP response
        ssize_t bytes_sent = write(client_fd, http_response, strlen(http_response));

        if (bytes_sent < 0)
        {
          perror("Response sent failed..!");
          close(client_fd);
          continue;
        }
        else
        {
          printf("HTTP response sent.\n");
        }

      } else {
        printf("Failed to parse request line\n");

        // Bad request
        char *error_response = 
          "HTTP/1.1 400 Bad Request\r\n"
          "Content-Type: text/plain\r\n"
          "Content-Length: 11\r\n"
          "\r\n"
          "Bad Request";

        write(client_fd, error_response, strlen(error_response));
      }
    }

    // Close the connection
    close(client_fd);
    printf("Client Disconnected\n\n");
  }

  // Close the connection
  close(server_fd);

  return 0;
}
