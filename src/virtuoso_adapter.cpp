#include "virtuoso_adapter.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

#include <sql.h>
#include <sqlext.h>

using namespace dbshell;
using std::unique_ptr;
using std::string;
using std::cerr;
using std::cout;
using std::endl;
using std::runtime_error;
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

virtuoso_adapter::virtuoso_adapter(std::string host, std::string username) throw(runtime_error) {
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
  string password;

  if (pointer == nullptr) {
    cout << "WARNING: No password for user »" << username << "« found in ~/.vtpass, this will likely NOT work!" << endl;
  } else {
    password = *pointer;
    cout << "Got password »" << password << "«." << endl;
  }

  string dsn = "saruman";
  cout << "Connecting to data source »" << dsn << "«." << endl;

  if (!SQL_SUCCEEDED(SQLConnect(connection, (SQLCHAR*) dsn.c_str(), SQL_NTS, (SQLCHAR*) username.c_str(), SQL_NTS, (SQLCHAR*) password.c_str(), SQL_NTS))) {
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

unique_ptr<table> virtuoso_adapter::query(string query) throw (std::runtime_error) {
  unique_ptr<table> table(unique_ptr<dbshell::table>(new dbshell::table()));
  return table;
}

void virtuoso_adapter::cancel() {
}
