#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <pqxx/pqxx>

namespace mylib {

/**
 * @brief Simple user structure representing a database record
 */
struct User {
    int id;
    std::string name;
    std::string email;
};

/**
 * @brief Database client for PostgreSQL operations
 * 
 * This class provides utility functions to interact with a PostgreSQL
 * database using the libpqxx library. It demonstrates best practices
 * for C++20 database interactions.
 */
class DatabaseClient {
public:
    /**
     * @brief Construct a new Database Client object
     * 
     * @param connection_string PostgreSQL connection string
     *        Example: "postgresql://user:password@localhost:5432/dbname"
     */
    explicit DatabaseClient(const std::string& connection_string);
    
    /**
     * @brief Destroy the Database Client object
     */
    ~DatabaseClient();
    
    // Delete copy constructor and assignment operator
    DatabaseClient(const DatabaseClient&) = delete;
    DatabaseClient& operator=(const DatabaseClient&) = delete;
    
    // Allow move semantics
    DatabaseClient(DatabaseClient&&) noexcept;
    DatabaseClient& operator=(DatabaseClient&&) noexcept;
    
    /**
     * @brief Query all users from the database
     * 
     * @param limit Maximum number of users to return
     * @param offset Number of users to skip
     * @return std::vector<User> List of users
     */
    std::vector<User> queryUsers(int limit = 100, int offset = 0);
    
    /**
     * @brief Get a specific user by ID
     * 
     * @param user_id The user ID to search for
     * @return std::optional<User> The user if found, std::nullopt otherwise
     */
    std::optional<User> getUser(int user_id);
    
    /**
     * @brief Add a new user to the database
     * 
     * @param name User's name
     * @param email User's email
     * @return int The ID of the newly created user
     */
    int addUser(const std::string& name, const std::string& email);
    
    /**
     * @brief Get the total count of users in the database
     * 
     * @return int Total number of users
     */
    int getUserCount();
    
    /**
     * @brief Initialize the database schema
     * 
     * Creates the users table if it doesn't exist
     */
    void initializeSchema();
    
    /**
     * @brief Check if the connection is active
     * 
     * @return true if connected, false otherwise
     */
    bool isConnected() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl_;
};

} // namespace mylib
