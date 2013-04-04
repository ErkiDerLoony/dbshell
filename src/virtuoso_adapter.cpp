#include "virtuoso_adapter.hpp"

#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <sstream>

using namespace dbshell;
using std::unique_ptr;
using std::string;
using std::cerr;
using std::cout;
using std::endl;
using std::runtime_error;
using std::stringstream;

void virtuoso_adapter::error() throw(runtime_error) {
  SQLCHAR* buffer = (SQLCHAR*) malloc(250 * sizeof(SQLCHAR));
  SQLCHAR* sqlstate = (SQLCHAR*) malloc(15 * sizeof(SQLCHAR));

  stringstream msg;
  cout << "Creating error message." << endl;

  while (SQLError(environment, connection, statement, sqlstate, NULL, buffer, 100, NULL) == SQL_SUCCESS) {
    cout << "Assembling buffer." << endl;
    msg << buffer;
    cout << "Assembling sqlstate." << endl;
    msg << sqlstate;
  }

  cout << "Cleaning up." << endl;
  free(buffer);
  free(sqlstate);

  cout << "Throwing error." << endl;
  throw runtime_error(msg.str());
}

virtuoso_adapter::virtuoso_adapter(std::string host, std::string username) throw(runtime_error) {
  cout << "Trying to create environment." << endl;

  if (!SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &environment)) {
    error();
  }

  /*
  if (!SQLSetEnvAttr(environment, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0)) {
    error();
  }
  */

  cout << "Trying to create connection." << endl;

  if (!SQLAllocHandle(SQL_HANDLE_DBC, environment, &connection)) {
    error();
  }
}

virtuoso_adapter::~virtuoso_adapter() {
}

unique_ptr<table> virtuoso_adapter::query(string query) throw (std::runtime_error) {
  unique_ptr<table> table(unique_ptr<dbshell::table>(new dbshell::table()));
  return table;
}

void virtuoso_adapter::cancel() {
}
