#pragma once

#include <string>
#include <memory>
#include <grpcpp/grpcpp.h>
#include "mylib/database_client.hpp"

namespace mylib {

/**
 * @brief gRPC server for database operations
 * 
 * This class provides a gRPC server that exposes database operations
 * through the DatabaseService interface. It demonstrates modern C++20
 * patterns and best practices for gRPC server implementation.
 */
class GrpcServer {
public:
    /**
     * @brief Construct a new Grpc Server object
     * 
     * @param server_address The address to bind the server to (e.g., "0.0.0.0:50051")
     * @param db_connection_string PostgreSQL connection string for the database client
     */
    explicit GrpcServer(const std::string& server_address, 
                       const std::string& db_connection_string);
    
    /**
     * @brief Destroy the Grpc Server object
     */
    ~GrpcServer();
    
    // Delete copy constructor and assignment operator
    GrpcServer(const GrpcServer&) = delete;
    GrpcServer& operator=(const GrpcServer&) = delete;
    
    // Allow move semantics
    GrpcServer(GrpcServer&&) noexcept;
    GrpcServer& operator=(GrpcServer&&) noexcept;
    
    /**
     * @brief Start the gRPC server
     * 
     * This method starts the server and blocks until Shutdown() is called.
     */
    void run();
    
    /**
     * @brief Shutdown the gRPC server
     * 
     * Initiates a graceful shutdown of the server.
     */
    void shutdown();
    
    /**
     * @brief Wait for the server to shutdown
     * 
     * Blocks until the server has completely shut down.
     */
    void wait();
    
    /**
     * @brief Check if the server is running
     * 
     * @return true if the server is running, false otherwise
     */
    bool isRunning() const;
    
    /**
     * @brief Get the server address
     * 
     * @return std::string The address the server is bound to
     */
    std::string getAddress() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl_;
};

} // namespace mylib
