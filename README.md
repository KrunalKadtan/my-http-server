
# Modular HTTP Server in Pure C

A robust, modular implementation of a TCP web server built from scratch using the standard **[Berkeley Sockets API](https://en.wikipedia.org/wiki/Berkeley_sockets)**. This project demonstrates how to structure a C project using separation of concerns, custom logging, and a Makefile build system.

## Objective

To create a maintainable, extensible HTTP server that runs on **Port 8080**. The project moves away from a single "monolithic" file into distinct modules:

1. **Server:** Handles business logic (Parsing, Routing, HTTP Responses).
2. **Socket:** Handles low-level network system calls.
3. **Logger:** Handles formatted output with timestamps and log levels.

## Project Structure

```text
my-http-server/
├── Makefile             # Build automation
├── src/
│   ├── server.c         # Main application logic
│   ├── socket.c         # Network wrapper functions
│   └── logger.c         # Logging implementation
├── include/
│   ├── socket.h         # Socket module interface
│   └── logger.h         # Logger module interface
└── obj/                 # Compiled object files (auto-generated)
```

## How to Run

This project uses a **Makefile** to automate compilation.

### 1. Build and Run

To compile and start the server immediately:

```bash
make run
```

### 2. Build Only

To compile the executable (`server`) without running it:

```bash
make
```

### 3. Clean Build

To remove all compiled object files and the executable:

```bash
make clean
```

## Testing the Server

Once the server is running (via `make run`), you can test it:

**Option A: Web Browser**

* **Home:** http://localhost:8080/
* **About:** http://localhost:8080/about

**Option B: Command Line**

```bash
curl -v http://localhost:8080/
```

## Technical Architecture

This project implements a **Modular Design Pattern** to improve readability and maintainability.

### 1. The Logger Module (`src/logger.c`)

* **Purpose:** Replaces standard `printf` with a structured logging system.
* **Features:**
* **Timestamps:** Automatically adds `[HH:MM:SS]` to every log.
* **Log Levels:** Supports `DEBUG`, `INFO`, `WARN`, `ERROR`.
* **Filtering:** Can be configured to ignore low-priority logs (e.g., only show ERRORs in production).



### 2. The Socket Module (`src/socket.c`)

* **Purpose:** Encapsulates low-level OS system calls (`socket`, `bind`, `listen`, `setsockopt`).
* **Benefits:**
* Hides the complexity of `struct sockaddr_in`.
* Automatically handles `SO_REUSEADDR` (fixing "Address already in use" errors).
* Provides a clean API: `create_server_socket(port)` and `accept_client(fd)`.



### 3. The Server Core (`src/server.c`)

* **Purpose:** Orchestrates the application.
* **Logic:**
* Initializes the Logger and Socket modules.
* Runs the main `while(1)` accept loop.
* **Signal Handling:** Catches `SIGINT` (Ctrl+C) to safely close sockets before exiting.
* **HTTP Logic:** Parses the raw request buffer, determines the route, and constructs the HTTP/1.1 response.



## 📊 Expected Output

**Server Terminal:**

```text
[14:30:01] [INFO] Stating HTTP server...
[14:30:01] [INFO] Server listening on http://localhost:8080
[14:30:01] [INFO] Press Ctrl+C to stop

[14:30:05] [DEBUG] Client connected (fd: 4)
[14:30:05] [INFO] Request: GET /about
[14:30:05] [DEBUG] Response sent
```

## 📝 License

This project is open source and intended for educational purposes.