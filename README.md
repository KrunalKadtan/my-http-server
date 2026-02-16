# Simple TCP Server in Pure C

A minimal implementation of a TCP server built from scratch using the standard **[Berkeley Sockets API](https://en.wikipedia.org/wiki/Berkeley_sockets)**. This project demonstrates the fundamental lifecycle of a network server.

## Objective

To accept a single incoming TCP connection on Port 8080, read a message from the client, reply with a confirmation, and then exit gracefully.

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
Waiting for a connection...
```

### 3. Test the Connection

Open a **new** terminal window (keep the server running in the first one) and use `netcat` (nc) or `curl` to connect to your server.

**Option A: Using Netcat (Recommended)**

```bash
nc localhost 8080
```

⚠️ Important: The server will now wait for you to type something!
Type a message (e.g., "Hello World") and press Enter.

## Expected Behavior

**On the Server Terminal:**
Once you send the message from the client, the server will read it, print it, send a reply, and terminate.

```text
Server listening on port 8080...
Waiting for a connection...
Client connected!
Received 12 bytes from client.
Client sent: Hello World

Sent 20 bytes to client.
```

**On the Client Terminal:**
You will see your message echoed back with a confirmation.

```text
Message received..!
```

## Specific Concepts Demonstrated

This code implements the 5-step TCP Server Lifecycle:

1. **`socket()`**: Creates the endpoint for communication.
2. **`struct sockaddr_in`**: Manually defines the IPv4 address and Port (using `htons` for byte-order conversion).
3. **`bind()`**: Assigns the address to the socket.
4. **`listen()`**: Marks the socket as passive (waiting for incoming connections).
5. **`accept()`**: Blocks execution until a client connects (the 3-way handshake).
6. **`close()`**: Cleans up resources.
7. **`write()`**: Sends raw bytes (the welcome message) over the active socket file descriptor.
8. **`read()`**: Blocks again until the client sends data. It stores the incoming bytes into a zero-initialized buffer.

## License

This project is open source and intended for educational purposes.
