/*
 * © Copyright 2013 by Edgar Kalkowski <eMail@edgar-kalkowski.de>
 * 
 * This file is part of the program dbshell.
 * 
 * The program dbshell is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "postgres_adapter.hpp"

#include <sstream>
#include <iostream>

using std::cout;
using std::endl;
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
  cout << "Connecting to " << database << " on " << host << "." << endl;
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
    case 21: // int2
    case 23: // int4
    case 700: // float4
    case 701: // float8
    case 1700: // numeric
      result->add(buffer.str(), alignment_type::CENTER, L".");
      break;
    case 16: // bool
      result->add(buffer.str(), alignment_type::CENTER);
      break;
    case 25: // text
    case 1043: // varchar
    case 1082: // date
      result->add(buffer.str());
      break;
    default:
      buffer << " (unknown type = " << type << ")";
      result->add(buffer.str());
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
