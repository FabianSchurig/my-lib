#include "mylib/database_client.hpp"
#include <stdexcept>

namespace mylib {

// Private implementation (pImpl idiom)
class DatabaseClient::Impl {
public:
    explicit Impl(const std::string& connection_string)
        : connection_string_(connection_string)
        , connection_(std::make_unique<pqxx::connection>(connection_string)) {
        if (!connection_->is_open()) {
            throw std::runtime_error("Failed to connect to database");
        }
    }
    
    std::string connection_string_;
    std::unique_ptr<pqxx::connection> connection_;
};

DatabaseClient::DatabaseClient(const std::string& connection_string)
    : pImpl_(std::make_unique<Impl>(connection_string)) {
}

DatabaseClient::~DatabaseClient() = default;

DatabaseClient::DatabaseClient(DatabaseClient&&) noexcept = default;
DatabaseClient& DatabaseClient::operator=(DatabaseClient&&) noexcept = default;

std::vector<User> DatabaseClient::queryUsers(int limit, int offset) {
    std::vector<User> users;
    
    try {
        pqxx::work txn(*pImpl_->connection_);
        
        std::string query = "SELECT id, name, email FROM users ORDER BY id LIMIT " + 
                          txn.quote(limit) + " OFFSET " + txn.quote(offset);
        
        pqxx::result result = txn.exec(query);
        
        for (const auto& row : result) {
            User user;
            user.id = row["id"].as<int>();
            user.name = row["name"].as<std::string>();
            user.email = row["email"].as<std::string>();
            users.push_back(std::move(user));
        }
        
        txn.commit();
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to query users: ") + e.what());
    }
    
    return users;
}

std::optional<User> DatabaseClient::getUser(int user_id) {
    try {
        pqxx::work txn(*pImpl_->connection_);
        
        std::string query = "SELECT id, name, email FROM users WHERE id = " + 
                          txn.quote(user_id);
        
        pqxx::result result = txn.exec(query);
        
        if (result.empty()) {
            txn.commit();
            return std::nullopt;
        }
        
        User user;
        user.id = result[0]["id"].as<int>();
        user.name = result[0]["name"].as<std::string>();
        user.email = result[0]["email"].as<std::string>();
        
        txn.commit();
        return user;
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to get user: ") + e.what());
    }
}

int DatabaseClient::addUser(const std::string& name, const std::string& email) {
    try {
        pqxx::work txn(*pImpl_->connection_);
        
        std::string query = "INSERT INTO users (name, email) VALUES (" +
                          txn.quote(name) + ", " + txn.quote(email) + 
                          ") RETURNING id";
        
        pqxx::result result = txn.exec(query);
        
        int user_id = result[0]["id"].as<int>();
        
        txn.commit();
        return user_id;
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to add user: ") + e.what());
    }
}

int DatabaseClient::getUserCount() {
    try {
        pqxx::work txn(*pImpl_->connection_);
        
        pqxx::result result = txn.exec("SELECT COUNT(*) as count FROM users");
        
        int count = result[0]["count"].as<int>();
        
        txn.commit();
        return count;
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to get user count: ") + e.what());
    }
}

void DatabaseClient::initializeSchema() {
    try {
        pqxx::work txn(*pImpl_->connection_);
        
        txn.exec(R"(
            CREATE TABLE IF NOT EXISTS users (
                id SERIAL PRIMARY KEY,
                name VARCHAR(255) NOT NULL,
                email VARCHAR(255) NOT NULL UNIQUE,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        )");
        
        txn.commit();
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to initialize schema: ") + e.what());
    }
}

bool DatabaseClient::isConnected() const {
    return pImpl_->connection_ && pImpl_->connection_->is_open();
}

} // namespace mylib
