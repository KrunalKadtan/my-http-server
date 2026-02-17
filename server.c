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
    }
    else if (bytes_read == 0)
    {
      printf("Client Disconnected.\n");
    }
    else
    {
      // bytes_read > 0 : We received data
      printf("\nReceived %zd bytes from client.\n", bytes_read);
      printf("\nClient sent:\n%s\n", buffer);

      // Build HTTP response
      char http_response[2048];
      char *body = "Hello, World!\n";
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
      }
      else
      {
        printf("HTTP response sent.\n");
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
