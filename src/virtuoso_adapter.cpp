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

virtuoso_adapter::virtuoso_adapter(std::string host, std::string username) throw(runtime_error) {

  if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &environment))) {
    throw runtime_error("Error creating environment!");
  }

  if (!SQL_SUCCEEDED(SQLSetEnvAttr(environment, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0))) {
    throw runtime_error("Error setting ODBC version!");
  }

  if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_DBC, environment, &connection))) {
    throw runtime_error("Error creating connection!");
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
