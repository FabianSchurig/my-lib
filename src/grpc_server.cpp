#include "mylib/grpc_server.hpp"
#include "database_service.grpc.pb.h"
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/health_check_service_interface.h>

namespace mylib {

// Service implementation
class DatabaseServiceImpl final : public ::mylib::DatabaseService::Service {
public:
    explicit DatabaseServiceImpl(std::shared_ptr<DatabaseClient> db_client)
        : db_client_(std::move(db_client)) {}
    
    grpc::Status QueryUsers(grpc::ServerContext* context,
                           const ::mylib::QueryUsersRequest* request,
                           ::mylib::QueryUsersResponse* response) override {
        try {
            auto users = db_client_->queryUsers(request->limit(), request->offset());
            
            for (const auto& user : users) {
                auto* user_msg = response->add_users();
                user_msg->set_id(user.id);
                user_msg->set_name(user.name);
                user_msg->set_email(user.email);
            }
            
            response->set_total_count(db_client_->getUserCount());
            
            return grpc::Status::OK;
        } catch (const std::exception& e) {
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }
    
    grpc::Status GetUser(grpc::ServerContext* context,
                        const ::mylib::GetUserRequest* request,
                        ::mylib::GetUserResponse* response) override {
        try {
            auto user_opt = db_client_->getUser(request->user_id());
            
            if (user_opt.has_value()) {
                auto* user_msg = response->mutable_user();
                user_msg->set_id(user_opt->id);
                user_msg->set_name(user_opt->name);
                user_msg->set_email(user_opt->email);
                response->set_found(true);
            } else {
                response->set_found(false);
            }
            
            return grpc::Status::OK;
        } catch (const std::exception& e) {
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }
    
    grpc::Status AddUser(grpc::ServerContext* context,
                        const ::mylib::AddUserRequest* request,
                        ::mylib::AddUserResponse* response) override {
        try {
            int user_id = db_client_->addUser(request->name(), request->email());
            
            response->set_user_id(user_id);
            response->set_success(true);
            response->set_message("User added successfully");
            
            return grpc::Status::OK;
        } catch (const std::exception& e) {
            response->set_success(false);
            response->set_message(std::string("Failed to add user: ") + e.what());
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }

private:
    std::shared_ptr<DatabaseClient> db_client_;
};

// Private implementation
class GrpcServer::Impl {
public:
    Impl(const std::string& server_address, const std::string& db_connection_string)
        : server_address_(server_address)
        , db_client_(std::make_shared<DatabaseClient>(db_connection_string))
        , service_(std::make_unique<DatabaseServiceImpl>(db_client_))
        , running_(false) {
        
        // Initialize database schema
        db_client_->initializeSchema();
    }
    
    void run() {
        grpc::EnableDefaultHealthCheckService(true);
        grpc::reflection::InitProtoReflectionServerBuilderPlugin();
        
        grpc::ServerBuilder builder;
        builder.AddListeningPort(server_address_, grpc::InsecureServerCredentials());
        builder.RegisterService(service_.get());
        
        server_ = builder.BuildAndStart();
        
        if (server_) {
            running_ = true;
        }
    }
    
    void shutdown() {
        if (server_) {
            server_->Shutdown();
            running_ = false;
        }
    }
    
    void wait() {
        if (server_) {
            server_->Wait();
        }
    }
    
    std::string server_address_;
    std::shared_ptr<DatabaseClient> db_client_;
    std::unique_ptr<DatabaseServiceImpl> service_;
    std::unique_ptr<grpc::Server> server_;
    bool running_;
};

GrpcServer::GrpcServer(const std::string& server_address, 
                       const std::string& db_connection_string)
    : pImpl_(std::make_unique<Impl>(server_address, db_connection_string)) {
}

GrpcServer::~GrpcServer() {
    if (pImpl_ && pImpl_->running_) {
        pImpl_->shutdown();
    }
}

GrpcServer::GrpcServer(GrpcServer&&) noexcept = default;
GrpcServer& GrpcServer::operator=(GrpcServer&&) noexcept = default;

void GrpcServer::run() {
    pImpl_->run();
}

void GrpcServer::shutdown() {
    pImpl_->shutdown();
}

void GrpcServer::wait() {
    pImpl_->wait();
}

bool GrpcServer::isRunning() const {
    return pImpl_->running_;
}

std::string GrpcServer::getAddress() const {
    return pImpl_->server_address_;
}

} // namespace mylib
