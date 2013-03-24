#include "table.hpp"

#include <iostream>
#include <sstream>

using namespace dbshell;
using std::stringstream;
using std::string;
using std::shared_ptr;
using std::vector;
using std::cout;
using std::endl;

column::column(std::string name) : _name(name) {
  if (table::debug) cout << "Column »" << name << "« created." << endl;
}

column::~column() {
  if (table::debug) cout << "Column »" << name() << "« destroyed." << endl;
}

string column::name() {
  return _name;
}

double_column::double_column(string name) : column(name) {
  if (table::debug) cout << "Double column »" << name << "« created." << endl;
}

double_column::~double_column() {
  if (table::debug) cout << "Double column »" << name() << "« destroyed." << endl;
}

string double_column::name() {
  return column::name();
}

string double_column::format(double value) {
  stringstream s;
  s << value;
  return s.str();
}

long_column::long_column(std::string name) : column(name) {
  if (table::debug) cout << "Long column »" << name << "« created." << endl;
}

long_column::~long_column() {
  if (table::debug) cout << "Long column »" << name() << "« destroyed." << endl;
}

string long_column::name() {
  return column::name();
}

string long_column::format(long value) {
  stringstream s;
  s << value;
  return s.str();
}

bool table::debug = false;

table::table() {
  if (table::debug) cout << "Table created." << endl;
}

table::~table() {
  if (table::debug) cout << "Table destroyed." << endl;
}

int main(int argc, char** argv) {
  table::debug = true;
  shared_ptr<long_column> timestamp = shared_ptr<long_column>(new long_column("timestamp"));
  shared_ptr<double_column> value = shared_ptr<double_column>(new double_column("value"));

  vector<shared_ptr<column>> columns;
  columns.push_back(timestamp);
  columns.push_back(value);

  //table table;
  //table.add_column(shared_ptr<column>(new long_column("timestamp")));
  //table.add_column(shared_ptr<column>(new double_column("value")));
  return 0;
}
