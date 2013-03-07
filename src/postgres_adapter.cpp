#include "postgres_adapter.hpp"

#include <stdexcept>
#include <sstream>
#include <iostream>

postgres_adapter::postgres_adapter(std::string host, std::string username, std::string database)
  : _connection(nullptr), _host(host), _username(username), _database(database) {
  std::cout << "PostgresAdapter created." << std::endl;
}

postgres_adapter::~postgres_adapter() {

  if (_connection != nullptr) {
    std::cout << "Disconnecting from database." << std::endl;
    PQfinish(_connection);
  }

  std::cout << "PostgresAdapter destroyed." << std::endl;
}

std::string postgres_adapter::query(std::string query) {

  if (_connection == nullptr) {
    std::cout << "Connecting to database." << std::endl;
    std::stringstream info;
    info << "host=" << _host << " user=" << _username << " dbname=" << _database;
    _connection = PQconnectdb(info.str().c_str());

    if (PQstatus(_connection) != CONNECTION_OK) {
      std::stringstream text;
      text << "Could not connect to " << _host << " (conninfo: " << info.str() << ")!";
      throw std::runtime_error(text.str());
    }
  }

  auto res = PQexec(_connection, query.c_str());

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    std::stringstream text;
    text << "Failed to execute query “" << query << "”";
    PQclear(res);
    throw std::runtime_error(text.str());
  }

  PQclear(res);

  return "";
}
