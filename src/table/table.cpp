#include "table.hpp"
#include "double_column.hpp"
#include "long_column.hpp"

#include <iostream>
#include <sstream>

using namespace dbshell;
using std::stringstream;
using std::string;
using std::shared_ptr;
using std::vector;
using std::cout;
using std::endl;

bool table::debug = false;

table::table() {
  if (table::debug) cout << "Table created." << endl;
}

table::~table() {
  if (table::debug) cout << "Table destroyed." << endl;
}

void table::add_column(shared_ptr<column> column) {
  _columns.push_back(column);
  if (table::debug) cout << "Column " << (*column) << " added." << endl;
}

int main(int argc, char** argv) {
  table::debug = true;
  shared_ptr<long_column> timestamp = shared_ptr<long_column>(new long_column("timestamp"));
  shared_ptr<double_column> value = shared_ptr<double_column>(new double_column("value"));

  table table;
  table.add_column(timestamp);
  table.add_column(value);
  return 0;
}
