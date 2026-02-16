# Simple TCP Server in Pure C

A minimal implementation of a TCP server built from scratch using the standard **[Berkeley Sockets API](https://en.wikipedia.org/wiki/Berkeley_sockets)**. This project demonstrates the fundamental lifecycle of a network server.

## Objective

To accept a single incoming TCP connection on **Port 8080**, acknowledge it, and then exit gracefully.

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

**Option B: Using Curl**

```bash
curl -v telnet://localhost:8080

```

## Expected Behavior

**On the Server Terminal:**
Once you trigger the connection from the second terminal, the server will unblock, print the success message, and terminate.

```text
Server listening on port 8080...
Waiting for a connection...
Client connected!

```

**On the Client Terminal:**
You will see that the connection was opened and then immediately closed by the server.

## Specific Concepts Demonstrated

This code implements the 5-step TCP Server Lifecycle:

1. **`socket()`**: Creates the endpoint for communication.
2. **`struct sockaddr_in`**: Manually defines the IPv4 address and Port (using `htons` for byte-order conversion).
3. **`bind()`**: Assigns the address to the socket.
4. **`listen()`**: Marks the socket as passive (waiting for incoming connections).
5. **`accept()`**: Blocks execution until a client connects (the 3-way handshake).

## License

This project is open source and intended for educational purposes.
