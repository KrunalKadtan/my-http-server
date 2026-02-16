# Simple TCP Server in Pure C

A minimal implementation of a TCP server built from scratch using the standard **[Berkeley Sockets API](https://en.wikipedia.org/wiki/Berkeley_sockets)**. This project demonstrates how to build a server that can handle multiple clients sequentially (one after another) without shutting down.

## Objective

To create a server that runs indefinitely, continuously accepting incoming connections on Port 8080. For each connection, it will:
1. Read a message from the client.
2. Print the message to the server console.
3. Reply with a confirmation.
4. Close the client connection and immediately wait for the next user.

## Prerequisites

* **OS:** Linux or macOS (POSIX-compliant system)
* **Compiler:** GCC or Clang
* **Tools:** Terminal access

## How to Run

### 1. Compile the Code

Open your terminal and navigate to the directory containing `server.c`. Run the following command to compile the source code into an executable named `server`.

```bash
gcc server.c -o server
```

### 2. Start the Server

Run the newly created executable. The server will start and wait for a connection.

```bash
./server
```

*Expected Output:*

```text
Server listening on port 8080...
Press Ctrl+C to stop

Waiting for next client...
```

### 3. Test the Connection (Multiple Times)

You can now connect, send a message, disconnect, and connect again!

**Terminal Window 2 (Client A):**

```bash
nc localhost 8080
# Type "Hello Server" and hit Enter
```

**Terminal Window 3 (Client B):**

```bash
nc localhost 8080
# Type "Greetings from Client B" and hit Enter
```

To stop the server, go to the server terminal and press Ctrl+C.

## Expected Behavior

**On the Server Terminal:**
You will see a running log of all clients that have connected.

```text
Server listening on port 8080...
Press Ctrl+C to stop

Waiting for next client...
Client connected!
Received 13 bytes from client.
Client sent: Hello Server
Sent 20 bytes to client.
Client Disconnected

Waiting for next client...
Client connected!
Received 24 bytes from client.
Client sent: Greetings from Client B
Sent 20 bytes to client.
Client Disconnected

Waiting for next client...
```

**On the Client Terminal:**
Each client receives the confirmation message before being disconnected.

```text
Message received..!
```

## Specific Concepts Demonstrated

1. **`socket()`**: Creates the endpoint for communication.
2. **`struct sockaddr_in`**: Manually defines the IPv4 address and Port (using `htons` for byte-order conversion).
3. **`bind()`**: Assigns the address to the socket.
4. **`listen()`**: Marks the socket as passive (waiting for incoming connections).
5. **`accept()`**: Blocks execution until a client connects (the 3-way handshake).
6. **`close()`**: Cleans up resources.
7. **`write()`**: Sends raw bytes (the welcome message) over the active socket file descriptor.
8. **`read()`**: Blocks again until the client sends data. It stores the incoming bytes into a zero-initialized buffer.
9. **`while(1)` Loop**: The core structure that keeps the server alive indefinitely.
10. **Sequential Processing**: Handling one client at a time. If Client A is connected, Client B waits in the OS queue until Client A finishes.
11. Resource Management:
    1.  `server_fd`: Kept open forever (the listening socket).
    2.  `client_fd`: Created and closed inside the loop for each individual session.
12. **Resilient Error Handling**: Using continue to skip failed connections without crashing the entire server.
13. **Buffer Clearing**: Re-initializing the data buffer for every new connection to prevent data leaks between clients.

## License

This project is open source and intended for educational purposes.
