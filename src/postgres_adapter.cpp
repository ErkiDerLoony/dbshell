#include "postgres_adapter.hpp"

#include <sstream>

using std::pair;
using std::vector;
using std::string;
using std::wstring;
using std::runtime_error;
using std::stringstream;
using std::wstringstream;
using std::unique_ptr;

namespace dbshell {

postgres_adapter::postgres_adapter(string host, string username, string database) throw(runtime_error)
  : _connection(nullptr), _host(host), _username(username), _database(database) {
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
    PQfinish(_connection);
  }
}

unique_ptr<table> postgres_adapter::query(string query) throw(runtime_error) {

  if (_connection == nullptr) {
    throw runtime_error("Not connected to any database!");
  }

  if (query == "\\d" || query == "\\dt") {
    query = "SELECT table_name, table_type FROM information_schema.tables WHERE table_schema IN (SELECT current_schema()) ORDER BY table_name";
  } else if (query.length() > 2 && query.substr(0, 3) == "\\d ") {
    query = "SELECT column_name, data_type FROM information_schema.columns WHERE table_name = '" + query.substr(3) + "'";
  }

  auto res = PQexec(_connection, query.c_str());

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    std::string text = PQresultErrorMessage(res);
    PQclear(res);
    throw runtime_error(text);
  }

  int nrows = PQntuples(res);
  int ncols = PQnfields(res);

  unique_ptr<table> result = unique_ptr<table>(new table());

  for (int i = 0; i < ncols; i++) {
    wstringstream buffer;
    buffer << PQfname(res, i);
    Oid type = PQftype(res, i);

    switch (type) {
    case 20: // int8
    case 23: // int4
    case 1700: // numeric
      result->add(buffer.str(), alignment_type::RIGHT, L".");
      break;
    case 1043: // varchar
      result->add(buffer.str());
      break;
    default:
      buffer << " (unknown type = " << type << ")";
    }
  }

  for (int i = 0; i < nrows; i++) {
    vector<wstring> row;

    for (int j = 0; j < ncols; j++) {
      char* buffer = PQgetvalue(res, i, j);
      wchar_t wbuffer[1024];
      std::mbstowcs(wbuffer, buffer, 1024);
      row.push_back(wbuffer);
    }

    result->add(row);
  }

  PQclear(res);

  return result;
}

void postgres_adapter::cancel() {
  PGcancel* cancel = PQgetCancel(_connection);
  char buffer[512];
  PQcancel(cancel, buffer, 512);
  PQfreeCancel(cancel);
}

} /* namespace dbshell */
