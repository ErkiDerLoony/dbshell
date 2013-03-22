#include "table.hpp"

#include <iostream>

using namespace dbshell;
using std::string;
using std::shared_ptr;
using std::vector;
using std::cout;
using std::endl;

bool table::debug = false;

template<typename T>
column<T>::column(T value) : _value(value) {
}

template<typename T>
column<T>::~column() {
}

template<typename T>
void column<T>::print() {
  cout << _value << endl;
}

table::table() {
  if (table::debug) cout << "Table created." << endl;
}

table::~table() {
  if (table::debug) cout << "Table destroyed." << endl;
}

int main(int argc, char** argv) {
  table::debug = true;
  column<long> timestamp(123);
  column<double> value(2.5);
  timestamp.print();
  value.print();

  vector<void*> columns;
  columns.push_back(&timestamp);

  //table table;
  //table.add_column(shared_ptr<column>(new long_column("timestamp")));
  //table.add_column(shared_ptr<column>(new double_column("value")));
  return 0;
}
