#include "mylib/database_client.hpp"
#include <stdexcept>

namespace mylib {

DatabaseClient::DatabaseClient(const std::string& connection_string)
    : conn_(std::make_unique<pqxx::connection>(connection_string)) {
    if (!conn_->is_open()) {
        throw std::runtime_error("Failed to connect to database");
    }
}

std::vector<User> DatabaseClient::queryUsers(int limit, int offset) {
    std::vector<User> users;
    pqxx::work txn(*conn_);
    
    auto result = txn.exec_params(
        "SELECT id, name, email FROM users ORDER BY id LIMIT $1 OFFSET $2",
        limit, offset
    );
    
    for (const auto& row : result) {
        users.push_back({
            row["id"].as<int>(),
            row["name"].as<std::string>(),
            row["email"].as<std::string>()
        });
    }
    
    txn.commit();
    return users;
}

std::optional<User> DatabaseClient::getUser(int user_id) {
    pqxx::work txn(*conn_);
    
    auto result = txn.exec_params(
        "SELECT id, name, email FROM users WHERE id = $1",
        user_id
    );
    
    if (result.empty()) {
        txn.commit();
        return std::nullopt;
    }
    
    User user{
        result[0]["id"].as<int>(),
        result[0]["name"].as<std::string>(),
        result[0]["email"].as<std::string>()
    };
    
    txn.commit();
    return user;
}

int DatabaseClient::addUser(const std::string& name, const std::string& email) {
    pqxx::work txn(*conn_);
    
    auto result = txn.exec_params(
        "INSERT INTO users (name, email) VALUES ($1, $2) RETURNING id",
        name, email
    );
    
    int user_id = result[0]["id"].as<int>();
    txn.commit();
    return user_id;
}

void DatabaseClient::initializeSchema() {
    pqxx::work txn(*conn_);
    
    txn.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id SERIAL PRIMARY KEY,
            name VARCHAR(255) NOT NULL,
            email VARCHAR(255) NOT NULL UNIQUE
        )
    )");
    
    txn.commit();
}

} // namespace mylib
