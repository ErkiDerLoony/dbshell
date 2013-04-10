#include "virtuoso_adapter.hpp"
#include "virtuoso_defines.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include <sql.h>
#include <sqlext.h>
#include <sqlucode.h>

using namespace dbshell;
using std::unique_ptr;
using std::string;
using std::wstring;
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
  cout << "Creating virtuoso adapter." << endl;
  cout << "Allocating environment." << endl;

  if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &environment))) {
    throw runtime_error("Error creating environment!");
  }

  cout << "Adapting ODBC version." << endl;

  if (!SQL_SUCCEEDED(SQLSetEnvAttr(environment, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0))) {
    throw runtime_error("Error setting ODBC version!");
  }

  cout << "Creating connection handle." << endl;

  if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_DBC, environment, &connection))) {
    throw runtime_error("Error creating connection!");
  }

  cout << "Loading password." << endl;
  unique_ptr<string> pointer = pwd(username);
  stringstream password;

  if (pointer == nullptr) {
    cout << "WARNING: No password for user »" << username << "« found in ~/.vtpass, this will likely NOT work!" << endl;
  } else {

    for (uint i = 0; i < pointer->length(); i++) {
      password << "*";
    }

    cout << "Got password »" << password.str() << "«." << endl;
  }

  cout << "Connecting to data source »" << dsn << "«." << endl;

  if (!SQL_SUCCEEDED(SQLConnect(connection, (SQLCHAR*) dsn.c_str(), SQL_NTS, (SQLCHAR*) username.c_str(),
                                SQL_NTS, (SQLCHAR*) pointer->c_str(), SQL_NTS))) {
    throw runtime_error("Error connecting to data source!");
  }

  cout << "Allocating statement." << endl;

  if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, connection, &statement))) {
    throw runtime_error("Error allocating statement!");
  }
}

virtuoso_adapter::~virtuoso_adapter() {

  if (statement) {
    cout << "Releasing statement." << endl;
    SQLFreeHandle(SQL_HANDLE_STMT, statement);
  }

  if (connection) {
    cout << "Disconnecting and releasing connection." << endl;
    SQLDisconnect(connection);
    SQLFreeHandle(SQL_HANDLE_DBC, connection);
  }

  if (environment) {
    cout << "Releasing environment." << endl;
    SQLFreeHandle(SQL_HANDLE_ENV, environment);
  }

  cout << "Virtuoso adapter destroyed." << endl;
}

unique_ptr<table> virtuoso_adapter::query(string query) throw (runtime_error) {

  if (query == "\\p" || query == "prefixes") {
    return _prefixes.format_as_table();
  }

  string prefix = query.substr(0, 6);

  if (prefix == "sparql" || prefix == "SPARQL" || prefix == "Sparql") {
    query = "SPARQL " + _prefixes.format() + " " + query.substr(6);
  } else {
    query = "SPARQL " + _prefixes.format() + " " + query;
  }

  unique_ptr<table> table(unique_ptr<table>(new dbshell::table()));

  if (SQLExecDirect(statement, (SQLCHAR*) query.c_str(), SQL_NTS) != SQL_SUCCESS) {
    throw runtime_error("Error executing query!");
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
    rc = SQLNumResultCols (statement, &numCols);

    if (!SQL_SUCCEEDED (rc)) {
      SQLCloseCursor(statement);
      throw runtime_error("Could not get number of result columns!");
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
        throw runtime_error("Could not find out column name!");
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
        throw runtime_error("Could not fetch any data!");
      }

      vector<wstring> row;

      for (colNum = 1; colNum <= numCols; colNum++) {
        wchar_t buf[2048];
        SQLINTEGER len;
        int flag, dvtype;

        rc = SQLGetData(statement, colNum, SQL_C_WCHAR, fetchBuffer, sizeof(fetchBuffer), &colIndicator);

        if (!SQL_SUCCEEDED (rc)) {
          SQLCloseCursor(statement);
          throw runtime_error("Could not get column data!");
        }

        rc = SQLGetStmtAttr(statement, SQL_ATTR_IMP_ROW_DESC, &hdesc, SQL_IS_POINTER, NULL);

        if (!SQL_SUCCEEDED (rc)) {
          SQLCloseCursor(statement);
          throw runtime_error("Could not create column attribute handle!");
        }

        rc = SQLGetDescField (hdesc, colNum, SQL_DESC_COL_DV_TYPE, &dvtype, SQL_IS_INTEGER, NULL);

        if (!SQL_SUCCEEDED (rc)) {
          SQLCloseCursor(statement);
          throw runtime_error("Could not get column data type!");
        }

        rc = SQLGetDescField (hdesc, colNum, SQL_DESC_COL_BOX_FLAGS, &flag, SQL_IS_INTEGER, NULL);

        if (!SQL_SUCCEEDED (rc)) {
          SQLCloseCursor(statement);
          throw runtime_error("Could not get column flags!");
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
            rc = SQLGetDescField(hdesc, colNum, SQL_DESC_COL_LITERAL_LANG, buf, sizeof(buf), &len);

            if (!SQL_SUCCEEDED(rc)) {
              SQLCloseCursor(statement);
              throw runtime_error("Could not get literal language!");
            }

            if (len) {
              cell << "@" << buf;
            }

            rc = SQLGetDescField(hdesc, colNum, SQL_DESC_COL_LITERAL_TYPE, buf, sizeof(buf), &len);

            if (!SQL_SUCCEEDED (rc)) {
              SQLCloseCursor(statement);
              throw runtime_error("Could not get literal type!");
            }

            if (len) {
              cell << "^^" << buf;
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
      cout << "Query cancelled." << endl;
    }
  }
}
