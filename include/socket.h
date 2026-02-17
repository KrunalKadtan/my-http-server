#ifndef SOCKET_H
#define SOCKET_H

// Create and Configure a server socket
int create_server_socket (int port);

// Accept a client connection
int accept_client (int server_fd);

#endif  // SOCKET_H
