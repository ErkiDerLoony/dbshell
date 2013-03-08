#include "postgres_adapter.hpp"

#include <sstream>
#include <iostream>

postgres_adapter::postgres_adapter(std::string host, std::string username, std::string database) throw(std::runtime_error)
  : _connection(nullptr), _host(host), _username(username), _database(database) {
  std::cout << "PostgresAdapter created." << std::endl;
  std::stringstream info;
  info << "host=" << _host << " user=" << _username << " dbname=" << _database;
  _connection = PQconnectdb(info.str().c_str());

  if (PQstatus(_connection) != CONNECTION_OK) {
    std::stringstream text;
    text << "Could not connect to " << _host << " (conninfo: " << info.str() << ")!";
    throw std::runtime_error(text.str());
  }
}

postgres_adapter::~postgres_adapter() {

  if (_connection != nullptr) {
    std::cout << "Disconnecting from database." << std::endl;
    PQfinish(_connection);
  }

  std::cout << "PostgresAdapter destroyed." << std::endl;
}

std::string postgres_adapter::query(std::string query) throw(std::runtime_error) {

  if (_connection == nullptr) {
    std::cout << "Connecting to database." << std::endl;
  }

  auto res = PQexec(_connection, query.c_str());

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    std::stringstream text;
    text << "Failed to execute query “" << query << "”";
    PQclear(res);
    throw std::runtime_error(text.str());
  }

  int rows = PQntuples(res);
  int cols = PQnfields(res);

  for (int i = 0; i < cols; i++) {
    std::string label = PQfname(res, i);
    std::cout << label;

    if (i < cols - 1) {
      std::cout << ", ";
    } else {
      std::cout << std::endl;
    }
  }

  for (int i = 0; i < rows; i++) {

    for (int j = 0; j < cols; j++) {
      std::string cell = PQgetvalue(res, i, j);
      std::cout << cell;

      if (j < cols - 1) {
        std::cout << ", ";
      } else {
        std::cout << std::endl;
      }
    }
  }

  PQclear(res);

  return "";
}
