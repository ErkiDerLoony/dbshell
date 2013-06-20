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

virtuoso_adapter::virtuoso_adapter(string dsn, string username) throw(runtime_error) {
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

unique_ptr<table> virtuoso_adapter::query(string query) throw (runtime_error) {

  if (query == "\\p" || query == "prefixes") {
    return _prefixes.format_as_table();
  } else if (query == "\\g") {
    query = "SELECT DISTINCT ?graph WHERE { GRAPH ?graph { ?s ?p ?o } }";
  } else if (query == "\\h" || query == "\\?") {
    wcout << "\\g       List graphs in the currently connected database." << endl;
    wcout << "(\\h|\\?)  Print this helpful message." << endl;
    wcout << "\\p       Print prefix table." << endl;
    wcout << "\\q       Disconnect and exit." << endl;
    return unique_ptr<table>(nullptr);
  }

  string prefix = query.substr(0, 6);

  if (prefix == "sparql" || prefix == "SPARQL" || prefix == "Sparql") {
    query = "SPARQL " + _prefixes.format() + " " + query.substr(6);
  } else {
    query = "SPARQL " + _prefixes.format() + " " + query;
  }

  unique_ptr<table> table(unique_ptr<table>(new dbshell::table()));

  if (SQLExecDirect(statement, (SQLCHAR*) query.c_str(), SQL_NTS) != SQL_SUCCESS) {
    throw runtime_error("Error executing query!\n");
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
