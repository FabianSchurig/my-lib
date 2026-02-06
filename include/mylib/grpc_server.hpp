#pragma once

#include <string>
#include <memory>
#include <grpcpp/grpcpp.h>

namespace mylib {

class DatabaseClient;

class GrpcServer {
public:
    GrpcServer(const std::string& server_address, 
               const std::string& db_connection_string);
    ~GrpcServer();
    
    void run();
    void shutdown();
    void wait();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace mylib
