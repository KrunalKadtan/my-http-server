# Simple TCP Server in Pure C

A minimal implementation of a TCP server built from scratch using the standard **[Berkeley Sockets API](https://en.wikipedia.org/wiki/Berkeley_sockets)**. This project demonstrates how to build a server that communicates using the HTTP/1.1 Protocol, parses incoming requests, and routes them to different logical "pages".

## Objective

To create a server that runs indefinitely on **Port 8080**, accepting connections from web browsers. For each connection, it will:
1. **Receive** a raw HTTP request.
2. **Parse** the Request Line to extract the **Method** (GET), **Path** (/about), and **Version** (HTTP/1.1).
3. **Route** the request to the correct content based on the URL path.
4. **Construct** a valid HTTP/1.1 response (Status + Headers + Body).
5. **Serve** the response and close the connection.

## Prerequisites

* **OS:** Linux or macOS (POSIX-compliant system)
* **Compiler:** GCC or Clang
* **Tools:** Terminal access, Web Browser (Chrome/Firefox/Safari)

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
Server listening on http://localhost:8080
Press Ctrl+C to stop

Waiting for next client...
```

### 3. Test the Routing

You can now test different URLs to see the routing logic in action.

**Option A: Web Browser**

- **Home Page**: http://localhost:8080/ $\rightarrow$ Displays "Home Page"
- **About Page**: http://localhost:8080/about $\rightarrow$ Displays "About Page"
- **Error Page**: http://localhost:8080/pizza $\rightarrow$ Displays "404 Not Found"

**Option B: Command Line (curl)**

```bash
curl -v http://localhost:8080/about
```

To stop the server, go to the server terminal and press Ctrl+C.

## Expected Behavior

**On the Server Terminal:**
You will see a running log of all clients that have connected.

```text
Waiting for next client...
Client connected!

Received 78 bytes from client.

Client sent:
GET /about HTTP/1.1
Host: localhost:8080
User-Agent: curl/8.16.0
Accept: */*


Method: GET
Path: /about
Version: HTTP/1.1
HTTP response sent.
Client Disconnected
```

**On the Client Terminal:**

```text
About Page
```

## Technical Architecture

Instead of relying on external frameworks, this project manually implements the core components of a web server.

**1. The Execution Model (Iterative)**
  - **Synchronous Processing**: The server handles one client at a time using a main `while(1)` loop.
  - **Blocking I/O**: Utilizes blocking system calls (`accept`, `read`) to pause execution until network events occur, ensuring CPU efficiency during idle times.
  - **Resiliency**: Implements error handling with `continue` statements to ensure a single failed connection does not crash the main server process.
**2. Request Parsing & Routing**
The server acts as an interpreter for the HTTP text protocol:
  - **Parsing**: Uses `sscanf` to break the first line of the request (e.g., `GET /about HTTP/1.1`) into three distinct strings: Method, Path, and Version.
  - **Routing Logic**: Uses `strcmp` (String Compare) to match the extracted Path against known routes (`/`, `/about`).
  - **Fallback Handling**: Automatically defaults to a "404 Not Found" response if the path does not match any known route.
**3. Protocol Implementation (HTTP/1.1)**
The server moves beyond raw binary streams to implement application-layer logic:
  - **Response Construction**: Manually formats HTTP headers (Status Code, Content-Type, Content-Length) using `sprintf`.
  - **Protocol Compliance**: Strictly adheres to HTTP standards, including the required \r\n line endings and double-CRLF separator between headers and body.

**4. Memory & Resource Management**
  - **Buffer Safety**: Uses explicit zero-initialization (`{0}`) for data buffers to prevent memory leaks or garbage data in strings.
  - **Descriptor Management**: distinct separation between the **Listening Socket** (Server Lifecycle) and **Connection Socket** (Request Lifecycle).

## License

This project is open source and intended for educational purposes.