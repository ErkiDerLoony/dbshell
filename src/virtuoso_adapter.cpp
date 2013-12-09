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

#include "virtuoso_adapter.hpp"
#include "virtuoso_defines.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdio>

#include <sql.h>
#include <sqlext.h>
#include <sqlucode.h>

using namespace dbshell;
using std::unique_ptr;
using std::string;
using std::wstring;
using std::cin;
using std::cerr;
using std::wcout;
using std::cout;
using std::endl;
using std::vector;
using std::runtime_error;
using std::wstringstream;
using std::stringstream;
using std::ifstream;

unique_ptr<string> virtuoso_adapter::pwd(const std::string& username) {
  ifstream file("/home/edgar/.vtpass");

  if (file.is_open()) {
    string line;

    while (file.good()) {
      getline(file, line);

      if (line.substr(0, username.length()) == username) {
        line = line.substr(username.length());

        if (line.substr(0, 1) == " ") {
          line = line.substr(1);
        }

        return unique_ptr<string>(new string(line));
      }
    }
  }

  return unique_ptr<string>(nullptr);
}

virtuoso_adapter::virtuoso_adapter(string dsn, string username) throw(runtime_error) : _sparql_mode(true) {
  cout << "Connecting to " << dsn << "." << endl;

  if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &environment))) {
    throw runtime_error("Error creating environment!");
  }

  if (!SQL_SUCCEEDED(SQLSetEnvAttr(environment, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0))) {
    throw runtime_error("Error setting ODBC version!");
  }

  if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_DBC, environment, &connection))) {
    throw runtime_error("Error creating connection!");
  }

  unique_ptr<string> pointer = pwd(username);
  string password;
  vector<char> buffer;

  if (pointer == nullptr) {
    cerr << "WARNING: No password found in ~/.vtpass! This will likely not work!" << endl;
    password = "";
  } else {
    password = *pointer;
  }

  if (!SQL_SUCCEEDED(SQLConnect(connection, (SQLCHAR*) dsn.c_str(), SQL_NTS, (SQLCHAR*) username.c_str(),
                                SQL_NTS, (SQLCHAR*) password.c_str(), SQL_NTS))) {
    throw runtime_error("Error connecting to data source!");
  }

  if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, connection, &statement))) {
    throw runtime_error("Error allocating statement!");
  }
}

virtuoso_adapter::~virtuoso_adapter() {

  if (statement) {
    SQLFreeHandle(SQL_HANDLE_STMT, statement);
  }

  if (connection) {
    SQLDisconnect(connection);
    SQLFreeHandle(SQL_HANDLE_DBC, connection);
  }

  if (environment) {
    SQLFreeHandle(SQL_HANDLE_ENV, environment);
  }
}

void virtuoso_adapter::error() const throw(runtime_error) {
  SQLCHAR state[1024];
  SQLINTEGER native;
  const SQLSMALLINT buffer_length = 4096;
  SQLCHAR text[buffer_length];
  SQLSMALLINT text_length;
  SQLError(environment, connection, statement, state, &native, text, buffer_length, &text_length);
  stringstream buffer;
  buffer << text << endl;
  throw runtime_error(buffer.str());
}

unique_ptr<table> virtuoso_adapter::query(string query) throw (runtime_error) {
  bool one_time_sql = false;

  if (query.length() > 2 && query.substr(0, 2) == "\\a") {
    query = query.substr(3);
    auto pos = query.find(" ");

    if (pos == string::npos) {
      stringstream buffer;
      buffer << "Missing IRI!" << endl;
      throw new runtime_error(buffer.str());
    } else {
      string prefix = query.substr(0, pos);
      string iri = query.substr(pos + 1);

      if (iri.substr(0, 1) == "<") {
	iri = iri.substr(1);
      }

      if (iri.substr(iri.length() - 1) == ">") {
	iri = iri.substr(0, iri.length() - 1);
      }

      _prefixes.add(prefix, iri);
      wcout << "Added prefix " << prefix.c_str() << " with IRI " << iri.c_str() << "." << endl;
      return unique_ptr<table>(nullptr);
    }

  } else if (query.length() > 2 && (query.substr(0, 2) == "\\r")) {
    string prefix = query.substr(3);

    if (_prefixes.contains(prefix)) {
      _prefixes.remove(prefix);
      wcout << L"Deleted prefix " << prefix.c_str() << L"." << endl;
    } else {
      wcout << L"No such prefix: “" << prefix.c_str() << "”!" << endl;
    }

    return unique_ptr<table>(nullptr);
  } else if (query == "\\d") {
    one_time_sql = true;
    query = "SELECT table_schema, table_name, table_type FROM information_schema.tables ORDER BY table_schema, table_name";
  } else if (query.size() > 2 && query.substr(0, 3) == "\\d ") {
    one_time_sql = true;
    string name = query.substr(3);
    auto index = name.find(".");

    if (index != string::npos) {
      string schema = name.substr(0, index);
      name = name.substr(index + 1);

      if (name == "*") {
        query = "SELECT table_name, table_type FROM information_schema.tables WHERE table_schema = '" + schema + "' ORDER BY table_name";
      } else {
        query = "SELECT column_name, data_type FROM information_schema.columns WHERE table_name = '" + name + "' AND table_schema = '" + schema + "'";
      }

    } else {
      query = "SELECT column_name, data_type FROM information_schema.columns WHERE table_name = '" + name + "'";
    }

  } else if (query == "\\p") {
    return _prefixes.format_as_table();
  } else if (query == "\\g") {
    query = "SELECT DISTINCT ?graph WHERE { GRAPH ?graph { ?s ?p ?o } }";
  } else if (query == "\\s") {
    _sparql_mode = !_sparql_mode;

    if (_sparql_mode) {
      wcout << L"Toggled to SPARQL mode." << endl;
    } else {
      wcout << L"Toggled to SQL mode." << endl;
    }

    return unique_ptr<table>(nullptr);
  } else if (query == "\\h" || query == "\\?") {
    wcout << L"\\a <prefix> <iri>   Add a prefix to the prefix table." << endl;
    wcout << L"\\d (<schema>.)?<table>  Describe the columns of a table."
          << endl;
    wcout << L"\\g                  List graphs in the currently connected "
          << L"database." << endl;
    wcout << L"(\\h|\\?)             Print this helpful message." << endl;
    wcout << L"\\p                  Print prefix table." << endl;
    wcout << L"\\q                  Disconnect and exit." << endl;
    wcout << L"\\r <prefix>         Delete a prefix from the prefix table."
          << endl;
    wcout << L"\\s                  Toggle between SQL and SPARQL mode."
          << endl;
    return unique_ptr<table>(nullptr);
  }

  if (_sparql_mode && !one_time_sql) {
    string s = query;
    size_t index;
    int counter = 0;

    while ((index = s.find("FROM <")) != string::npos) {
      stringstream prefix;
      s = s.substr(index + 6);
      auto index2 = s.find(">");
      stringstream iri;
      iri << s.substr(0, index2) << "#";
      s = s.substr(index2 + 1);

      if (counter == 0) {
        prefix << ":";
      } else {
        prefix << "p" << counter << ":";
      }

      wcout << L"Adding prefix " << prefix.str().c_str() << L" for IRI " << iri.str().c_str() << L"." << endl;
      _prefixes.add(prefix.str(), iri.str());
      counter++;
    }
  }

  string prefix = query.substr(0, 6);

  if (_sparql_mode && !one_time_sql && prefix != "SPARQL" && prefix != "sparql") {
    query = "SPARQL " + _prefixes.format() + " " + query;
  }

  unique_ptr<table> table(unique_ptr<table>(new dbshell::table()));

  if (SQLExecDirect(statement, (SQLCHAR*) query.c_str(), SQL_NTS) != SQL_SUCCESS) {
    error();
  }

  wchar_t fetchBuffer[1000];
  short numCols = 0;
  short colNum;
  SQLLEN colIndicator;
  UDWORD totalSets;
  SQLHANDLE hdesc = SQL_NULL_HANDLE;
  SQLRETURN rc;

  totalSets = 0;

  do {
    rc = SQLNumResultCols(statement, &numCols);

    if (!SQL_SUCCEEDED(rc)) {
      SQLCloseCursor(statement);
      throw runtime_error("Could not get number of result columns!\n");
    }

    if (numCols == 0) {
      return table;
    }

    for (int col = 1; col <= numCols; col++) {
      wchar_t buffer[1000];
      SQLSMALLINT length;
      SQLLEN flag = 0;

      rc = SQLColAttributeW(statement, col, SQL_DESC_NAME, buffer, 1000, &length, &flag);

      if (!SQL_SUCCEEDED(rc)) {
        SQLCloseCursor(statement);
        throw runtime_error("Could not find out column name!\n");
      }

      table->add_column(buffer);
    }

    while (1) {
      rc = SQLFetch(statement);

      if (rc == SQL_NO_DATA_FOUND) {
        break;
      }

      if (!SQL_SUCCEEDED(rc)) {
        SQLCloseCursor(statement);
        throw runtime_error("Could not fetch any data!\n");
      }

      vector<wstring> row;

      for (colNum = 1; colNum <= numCols; colNum++) {
        int flag, dvtype;

        rc = SQLGetData(statement, colNum, SQL_C_WCHAR, fetchBuffer, sizeof(fetchBuffer), &colIndicator);

        if (!SQL_SUCCEEDED (rc)) {
          SQLCloseCursor(statement);
          throw runtime_error("Could not get column data!\n");
        }

        rc = SQLGetStmtAttr(statement, SQL_ATTR_IMP_ROW_DESC, &hdesc, SQL_IS_POINTER, NULL);

        if (!SQL_SUCCEEDED (rc)) {
          SQLCloseCursor(statement);
          throw runtime_error("Could not create column attribute handle!\n");
        }

        rc = SQLGetDescField (hdesc, colNum, SQL_DESC_COL_DV_TYPE, &dvtype, SQL_IS_INTEGER, NULL);

        if (!SQL_SUCCEEDED (rc)) {
          SQLCloseCursor(statement);
          throw runtime_error("Could not get column data type!\n");
        }

        rc = SQLGetDescField (hdesc, colNum, SQL_DESC_COL_BOX_FLAGS, &flag, SQL_IS_INTEGER, NULL);

        if (!SQL_SUCCEEDED (rc)) {
          SQLCloseCursor(statement);
          throw runtime_error("Could not get column flags!\n");
        }

        if (colIndicator == SQL_NULL_DATA) {
          row.push_back(L"");
        } else {
          wstringstream cell;

          if (flag & VIRTUOSO_BF_IRI) {
            cell << "<" << fetchBuffer << ">";
          } else if (dvtype == VIRTUOSO_DV_STRING || dvtype == VIRTUOSO_DV_RDF) {
            cell << "\"" << fetchBuffer << "\"";
          } else {
            cell << fetchBuffer;
          }

          if (dvtype == VIRTUOSO_DV_RDF) {
            char buff[2048];
            SQLINTEGER len;
            rc = SQLGetDescField(hdesc, colNum, SQL_DESC_COL_LITERAL_LANG, buff, sizeof(buff), &len);

            if (!SQL_SUCCEEDED(rc)) {
              SQLCloseCursor(statement);
              throw runtime_error("Could not get literal language!\n");
            }

            if (len) {
              cell << "@" << buff;
            }

            rc = SQLGetDescField(hdesc, colNum, SQL_DESC_COL_LITERAL_TYPE, buff, sizeof(buff), &len);

            if (!SQL_SUCCEEDED(rc)) {
              SQLCloseCursor(statement);
              throw runtime_error("Could not get literal type!\n");
            }

            if (len) {
              string suffix(buff);
              suffix = _prefixes.prefix(suffix);
              cell << "^^" << suffix.c_str();
            }
          }

          row.push_back(cell.str());
        }
      }

      table->add(row);
    }

    totalSets++;
  } while (SQLMoreResults(statement) == SQL_SUCCESS);

  _prefixes.prefix(*table);
  return table;
}

void virtuoso_adapter::cancel() {

  if (statement) {
    cout << "Cancelling query." << endl;
    SQLRETURN result = SQLCancel(statement);

    if (!SQL_SUCCEEDED(result)) {
      cout << "Query could not be cancelled!" << endl;

      int index = 1;
      SQLCHAR state[5];
      SQLINTEGER native;
      SQLCHAR text[1000];
      SQLSMALLINT length;

      while (true) {
        result = SQLGetDiagRec(SQL_HANDLE_STMT, statement, index++, state, &native, text, sizeof(text), &length);

        if (!SQL_SUCCEEDED(result)) {
          break;
        }

        cout << state << " " << text << " (native error code " << native << ")" << endl;
      }

      cout << "Please be patient." << endl;
    } else {
      statement = SQL_NULL_HANDLE;
      cout << "Query cancelled." << endl;
    }
  }
}
