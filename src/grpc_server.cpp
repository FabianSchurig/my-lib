#include "mylib/grpc_server.hpp"
#include "mylib/database_client.hpp"
#include "database_service.grpc.pb.h"

namespace mylib {

class DatabaseServiceImpl final : public ::mylib::DatabaseService::Service {
public:
    explicit DatabaseServiceImpl(std::shared_ptr<DatabaseClient> db)
        : db_(std::move(db)) {}
    
    grpc::Status QueryUsers(grpc::ServerContext*,
                           const ::mylib::QueryUsersRequest* request,
                           ::mylib::QueryUsersResponse* response) override {
        auto users = db_->queryUsers(request->limit(), request->offset());
        
        for (const auto& user : users) {
            auto* u = response->add_users();
            u->set_id(user.id);
            u->set_name(user.name);
            u->set_email(user.email);
        }
        
        return grpc::Status::OK;
    }
    
    grpc::Status GetUser(grpc::ServerContext*,
                        const ::mylib::GetUserRequest* request,
                        ::mylib::GetUserResponse* response) override {
        auto user = db_->getUser(request->user_id());
        
        if (user) {
            response->mutable_user()->set_id(user->id);
            response->mutable_user()->set_name(user->name);
            response->mutable_user()->set_email(user->email);
            response->set_found(true);
        } else {
            response->set_found(false);
        }
        
        return grpc::Status::OK;
    }
    
    grpc::Status AddUser(grpc::ServerContext*,
                        const ::mylib::AddUserRequest* request,
                        ::mylib::AddUserResponse* response) override {
        try {
            int id = db_->addUser(request->name(), request->email());
            response->set_user_id(id);
            response->set_success(true);
            return grpc::Status::OK;
        } catch (const std::exception& e) {
            response->set_success(false);
            response->set_message(e.what());
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }

private:
    std::shared_ptr<DatabaseClient> db_;
};

class GrpcServer::Impl {
public:
    Impl(const std::string& addr, const std::string& db_conn)
        : db_(std::make_shared<DatabaseClient>(db_conn))
        , service_(std::make_unique<DatabaseServiceImpl>(db_)) {
        db_->initializeSchema();
        
        grpc::ServerBuilder builder;
        builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
        builder.RegisterService(service_.get());
        server_ = builder.BuildAndStart();
    }
    
    std::shared_ptr<DatabaseClient> db_;
    std::unique_ptr<DatabaseServiceImpl> service_;
    std::unique_ptr<grpc::Server> server_;
};

GrpcServer::GrpcServer(const std::string& server_address, 
                       const std::string& db_connection_string)
    : impl_(std::make_unique<Impl>(server_address, db_connection_string)) {}

GrpcServer::~GrpcServer() {
    if (impl_ && impl_->server_) {
        impl_->server_->Shutdown();
    }
}

void GrpcServer::run() {}

void GrpcServer::shutdown() {
    if (impl_->server_) {
        impl_->server_->Shutdown();
    }
}

void GrpcServer::wait() {
    if (impl_->server_) {
        impl_->server_->Wait();
    }
}

} // namespace mylib
