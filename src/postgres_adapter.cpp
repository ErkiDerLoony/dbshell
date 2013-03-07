#include "postgres_adapter.hpp"
#include "connection_failed.hpp"
#include "query_failed.hpp"

#include <sstream>
#include <iostream>

postgres_adapter::postgres_adapter(std::string host, std::string username)
  : _connection(nullptr), _host(host), _username(username) {
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
    info << "dbname = " << _host;
    _connection = PQconnectdb(info.str().c_str());

    if (PQstatus(_connection) != CONNECTION_OK) {
      std::stringstream text;
      text << "Could not connect to " << _host << "!";
      throw std::runtime_error(text.str());
    }
  }

  auto res = PQexec(_connection, query.c_str());

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    std::stringstream text;
    text << "Failed to execute query “" << query << "”";
    throw query_failed(text.str());
  }

  return "";
}
