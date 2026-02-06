# Example Server Application (Not Built by Default)

This directory contains example code showing how to use the mylib library.

To build examples:

```bash
cmake -B build -DMYLIB_BUILD_EXAMPLES=ON
cmake --build build
```

## server_example.cpp

A simple gRPC server application that demonstrates:
- Creating a DatabaseClient connection
- Initializing the database schema
- Starting a GrpcServer
- Handling graceful shutdown

## client_example.cpp

A simple gRPC client application that demonstrates:
- Connecting to the DatabaseService
- Adding users
- Querying users
- Getting specific users

## Usage

Start the server:
```bash
# Set PostgreSQL connection string
export DB_CONNECTION="postgresql://user:password@localhost:5432/testdb"

# Run server
./server_example
```

In another terminal, run the client:
```bash
./client_example
```

## Note

These examples are provided for reference but are not built by default.
To create an application using mylib, follow the patterns shown in these examples.
