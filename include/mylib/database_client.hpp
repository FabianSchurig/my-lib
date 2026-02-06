#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <pqxx/pqxx>

namespace mylib {

struct User {
    int id;
    std::string name;
    std::string email;
};

class DatabaseClient {
public:
    explicit DatabaseClient(const std::string& connection_string);
    
    std::vector<User> queryUsers(int limit = 100, int offset = 0);
    std::optional<User> getUser(int user_id);
    int addUser(const std::string& name, const std::string& email);
    void initializeSchema();

private:
    std::unique_ptr<pqxx::connection> conn_;
};

} // namespace mylib
