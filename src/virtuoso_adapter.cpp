#include "virtuoso_adapter.hpp"

#include <sql.h>
#include <sqlext.h>
#include <iostream>

using namespace dbshell;
using std::vector;
using std::string;
using std::pair;
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

pair<vector<string>, vector<vector<string>>> virtuoso_adapter::query(string query) throw (std::runtime_error) {
  pair<vector<string>, vector<vector<string>>> p;
  return p;
}

void virtuoso_adapter::cancel() {
}
