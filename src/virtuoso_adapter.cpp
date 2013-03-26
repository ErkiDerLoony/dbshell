#include "virtuoso_adapter.hpp"

#include <sql.h>
#include <sqlext.h>
#include <iostream>

using namespace dbshell;
using std::unique_ptr;
using std::string;
using std::cout;
using std::endl;

virtuoso_adapter::virtuoso_adapter(std::string host, std::string username) {
  cout << "Creating virtuoso adapter." << endl;
  SQLHENV env;
  SQLRETURN result = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
  cout << "Virtuoso adapter created (code " << result << ")." << endl;
}

virtuoso_adapter::~virtuoso_adapter() {
  cout << "Destroying virtuoso adapter." << endl;
}

unique_ptr<table> virtuoso_adapter::query(string query) throw (std::runtime_error) {
  return unique_ptr<table>(new table());
}

void virtuoso_adapter::cancel() {
}
