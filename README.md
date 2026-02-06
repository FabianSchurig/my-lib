# mylib - Modern C++20 Library for gRPC and PostgreSQL

A modern C++20 library providing database and gRPC utility functions for PostgreSQL operations. This library demonstrates best practices for C++ library development with gRPC and database integration.

## Features

- **Modern C++20**: Uses latest C++ features including concepts, ranges, and coroutines support
- **gRPC Integration**: Provides a complete gRPC service for database operations
- **PostgreSQL Support**: Built on libpqxx for robust database connectivity
- **Best Practices**: Follows modern CMake practices, RAII, pImpl idiom, and move semantics
- **DevContainer Support**: Ready-to-use development environment
- **Yocto Integration**: Includes BitBake recipe for embedded Linux systems

## Components

### Database Client (`DatabaseClient`)

Provides utility functions for PostgreSQL database operations:
- Query users with pagination
- Get specific user by ID
- Add new users
- Automatic schema initialization

### gRPC Server (`GrpcServer`)

Exposes database operations through a gRPC service with three RPCs:
- `QueryUsers`: List all users with pagination
- `GetUser`: Retrieve a specific user by ID
- `AddUser`: Create a new user

### Database Schema

Simple users table:
```sql
CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    email VARCHAR(255) NOT NULL UNIQUE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

## Requirements

- CMake 3.20 or higher
- C++20 compatible compiler (GCC 11+, Clang 13+, MSVC 19.29+)
- Protocol Buffers 3.21+
- gRPC 1.50+
- libpqxx 7.7+
- PostgreSQL 12+

## Building

### Using CMake

```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Install
sudo cmake --install build
```

### Using DevContainer

This project includes a complete development container with all dependencies pre-installed:

1. Open the project in Visual Studio Code
2. Install the "Dev Containers" extension
3. Press F1 and select "Dev Containers: Reopen in Container"
4. Wait for the container to build

The container includes:
- GCC 11 with C++20 support
- CMake and Ninja build system
- gRPC and Protocol Buffers
- libpqxx and PostgreSQL client
- Development tools (GDB, Valgrind, clang-format, clang-tidy)

### Build Options

- `BUILD_SHARED_LIBS`: Build shared libraries (default: ON)
- `MYLIB_BUILD_TESTS`: Build test suite (default: OFF)
- `MYLIB_BUILD_EXAMPLES`: Build example applications (default: OFF)

Example:
```bash
cmake -B build -DMYLIB_BUILD_TESTS=ON -DMYLIB_BUILD_EXAMPLES=ON
cmake --build build
```

## Usage

### Database Client

```cpp
#include <mylib/database_client.hpp>

// Create a database client
mylib::DatabaseClient db("postgresql://user:password@localhost:5432/mydb");

// Initialize schema
db.initializeSchema();

// Add a user
int user_id = db.addUser("John Doe", "john@example.com");

// Query users
auto users = db.queryUsers(10, 0);  // limit 10, offset 0

// Get specific user
auto user = db.getUser(user_id);
if (user.has_value()) {
    std::cout << "User: " << user->name << " (" << user->email << ")\n";
}
```

### gRPC Server

```cpp
#include <mylib/grpc_server.hpp>

// Create and start server
mylib::GrpcServer server(
    "0.0.0.0:50051",
    "postgresql://user:password@localhost:5432/mydb"
);

server.run();
std::cout << "Server listening on " << server.getAddress() << std::endl;

// Wait for shutdown
server.wait();
```

## Yocto Integration

This library includes a BitBake recipe for integration into Yocto-based embedded Linux systems.

### Adding to Your Layer

1. Copy `yocto/mylib_1.0.0.bb` to your layer's recipes directory
2. Add dependencies to your layer configuration
3. Add to your image recipe:

```bitbake
IMAGE_INSTALL:append = " mylib"
```

### Recipe Details

- **Package Name**: mylib
- **Version**: 1.0.0
- **Dependencies**: grpc, protobuf, libpqxx, postgresql
- **License**: MIT

## Project Structure

```
mylib/
├── .devcontainer/          # DevContainer configuration
│   ├── devcontainer.json
│   └── Dockerfile
├── cmake/                  # CMake modules
│   └── mylibConfig.cmake.in
├── include/mylib/          # Public headers
│   ├── database_client.hpp
│   └── grpc_server.hpp
├── src/                    # Implementation files
│   ├── database_client.cpp
│   └── grpc_server.cpp
├── proto/                  # Protocol Buffer definitions
│   └── database_service.proto
├── yocto/                  # Yocto/BitBake recipes
│   └── mylib_1.0.0.bb
├── tests/                  # Test suite
├── examples/               # Example applications
└── CMakeLists.txt          # Main CMake configuration
```

## Architecture

The library follows modern C++ best practices:

- **pImpl Idiom**: Implementation details hidden behind opaque pointers
- **RAII**: Automatic resource management
- **Move Semantics**: Efficient resource transfer
- **Zero-copy where possible**: Minimizes unnecessary data copying
- **Exception Safety**: Strong exception safety guarantees
- **Modern CMake**: Target-based configuration with proper export support

## Protocol Buffer Interface

The gRPC service is defined in `proto/database_service.proto`:

```protobuf
service DatabaseService {
    rpc QueryUsers(QueryUsersRequest) returns (QueryUsersResponse);
    rpc GetUser(GetUserRequest) returns (GetUserResponse);
    rpc AddUser(AddUserRequest) returns (AddUserResponse);
}
```

## Development

### Code Style

- C++20 standard
- Google C++ Style Guide compatible
- Use clang-format for formatting
- Use clang-tidy for static analysis

### Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Run tests and linters
5. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Built with [gRPC](https://grpc.io/)
- Database access via [libpqxx](https://github.com/jtv/libpqxx)
- Protocol Buffers by [Google](https://developers.google.com/protocol-buffers)