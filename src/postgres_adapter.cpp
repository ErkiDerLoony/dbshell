#include "postgres_adapter.hpp"

#include <sstream>
#include <iostream>

using std::pair;
using std::vector;
using std::string;
using std::runtime_error;
using std::cout;
using std::endl;
using std::stringstream;

namespace dbshell {

postgres_adapter::postgres_adapter(string host, string username, string database) throw(runtime_error)
  : _connection(nullptr), _host(host), _username(username), _database(database) {
  cout << "PostgresAdapter created." << endl;
  stringstream info;
  info << "host=" << _host << " user=" << _username << " dbname=" << _database;
  _connection = PQconnectdb(info.str().c_str());

  if (PQstatus(_connection) != CONNECTION_OK) {
    stringstream text;
    text << "Could not connect to " << _host << " (conninfo: " << info.str() << ")!";
    throw runtime_error(text.str());
  }
}

postgres_adapter::~postgres_adapter() {

  if (_connection != nullptr) {
    cout << "Disconnecting from database." << endl;
    PQfinish(_connection);
  }

  cout << "PostgresAdapter destroyed." << endl;
}

pair<vector<string>, vector<vector<string>>> postgres_adapter::query(string query) throw(runtime_error) {

  if (_connection == nullptr) {
    cout << "Connecting to database." << endl;
  }

  auto res = PQexec(_connection, query.c_str());

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    stringstream text;
    text << "Failed to execute query “" << query << "”";
    PQclear(res);
    throw runtime_error(text.str());
  }

  int nrows = PQntuples(res);
  int ncols = PQnfields(res);

  vector<string> columns;
  vector<vector<string>> rows;

  for (int i = 0; i < ncols; i++) {
    columns.push_back(PQfname(res, i));
  }

  for (int i = 0; i < nrows; i++) {
    vector<string> row;

    for (int j = 0; j < ncols; j++) {
      row.push_back(PQgetvalue(res, i, j));
    }

    rows.push_back(row);
  }

  PQclear(res);

  return pair<vector<string>, vector<vector<string>>>(columns, rows);
}

} /* namespace dbshell */
