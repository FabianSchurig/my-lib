# mylib - C++20 Library for gRPC and PostgreSQL

A minimal C++20 library providing database and gRPC utilities for PostgreSQL operations.

## Features

- Modern C++20 with standard library features
- gRPC service for database operations
- PostgreSQL support via libpqxx
- DevContainer with pre-configured environment
- Yocto/BitBake recipe for embedded Linux

## Components

**DatabaseClient**: PostgreSQL operations (query, get, add users)  
**GrpcServer**: Exposes database operations via gRPC  
**Database Schema**: Simple users table (id, name, email)

## Quick Start

### Using DevContainer (Recommended)

1. Open in VS Code with Dev Containers extension
2. Reopen in container
3. Build:
```bash
cmake -B build && cmake --build build
```

### Manual Build

Requirements: CMake 3.20+, C++20 compiler, gRPC, Protocol Buffers, libpqxx

```bash
cmake -B build
cmake --build build
sudo cmake --install build
```

## Usage

### Database Client
```cpp
#include <mylib/database_client.hpp>

mylib::DatabaseClient db("postgresql://user:pass@localhost/mydb");
db.initializeSchema();

int id = db.addUser("John", "john@example.com");
auto users = db.queryUsers(10, 0);
auto user = db.getUser(id);
```

### gRPC Server
```cpp
#include <mylib/grpc_server.hpp>

mylib::GrpcServer server("0.0.0.0:50051", "postgresql://...");
server.wait();  // Blocks until shutdown
```

## Yocto Integration

Copy `yocto/mylib_1.0.0.bb` to your layer and add to your image:
```bitbake
IMAGE_INSTALL:append = " mylib"
```

## Project Structure

```
mylib/
├── include/mylib/          # Public headers
├── src/                    # Implementation
├── proto/                  # gRPC service definition
├── cmake/                  # CMake helpers
├── .devcontainer/          # Development container
└── yocto/                  # BitBake recipe
```

## License

MIT License - see LICENSE file for details.