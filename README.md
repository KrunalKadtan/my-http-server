# Simple TCP Server in Pure C

A minimal implementation of a TCP server built from scratch using the standard **[Berkeley Sockets API](https://en.wikipedia.org/wiki/Berkeley_sockets)**. This project demonstrates how to build a server that can handle multiple clients sequentially (one after another) without shutting down.

## Objective

To create a server that runs indefinitely on Port 8080, accepting connections from web browsers (or CLI tools). For each connection, it will:
1. Receive a raw HTTP request.
2. Parse (log) the incoming request headers.
3. Construct a valid HTTP/1.1 response (Status + Headers + Body).
4. Serve the response to the client and close the connection.

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

**Option A: Web Browser (Recommended)**

Open your browser and navigate to:
http://localhost:8080

You should see "Hello, World!" displayed on the page.

**Option B: Command Line**

```bash
curl -v http://localhost:8080
```

To stop the server, go to the server terminal and press Ctrl+C.

## Expected Behavior

**On the Server Terminal:**
You will see a running log of all clients that have connected.

```text
Server listening on http://localhost:8080
Press Ctrl+C to stop

Waiting for next client...
Client connected!

Received 77 bytes from client.

Client sent:
GET / HTTP/1.1
Host: localhost:8080
User-Agent: curl/8.5.0
Accept: */*


HTTP response sent.
Client Disconnected

Waiting for next client...
```

**On the Client Terminal:**
Each client receives the confirmation message before being disconnected.

```text
Hello, World!
```

## Technical Architecture

Instead of relying on external frameworks, this project manually implements the core components of a web server.

**1. The Execution Model (Iterative)**
  - **Synchronous Processing**: The server handles one client at a time using a main `while(1)` loop.
  - **Blocking I/O**: Utilizes blocking system calls (`accept`, `read`) to pause execution until network events occur, ensuring CPU efficiency during idle times.
  - **Resiliency**: Implements error handling with `continue` statements to ensure a single failed connection does not crash the main server process.
**2. Network Lifecycle (The Syscalls)**
The server manually manages the full TCP state machine:
  - **Setup**: `socket()` (Endpoint creation) $\rightarrow$ `bind()` (Address assignment) $\rightarrow$ `listen()` (Passive mode).
  - **Connection**: `accept()` performs the 3-way handshake and yields a dedicated file descriptor (`client_fd`) for the session.
  - **Teardown**: `close()` is called immediately after the response is sent to signal the end of the HTTP transaction.
**3. Protocol Implementation (HTTP/1.1)**
The server moves beyond raw binary streams to implement application-layer logic:
  - **Response Construction**: Manually formats HTTP headers (Status Code, Content-Type, Content-Length) using `sprintf`.
  - **Protocol Compliance**: Strictly adheres to HTTP standards, including the required \r\n line endings and double-CRLF separator between headers and body.

**4. Memory & Resource Management**
  - **Buffer Safety**: Uses explicit zero-initialization (`{0}`) for data buffers to prevent memory leaks or garbage data in strings.
  - **Descriptor Management**: distinct separation between the **Listening Socket** (Server Lifecycle) and **Connection Socket** (Request Lifecycle).

## License

This project is open source and intended for educational purposes.