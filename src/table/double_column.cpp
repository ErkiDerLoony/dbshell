#include "double_column.hpp"

#include <sstream>
#include <iostream>

using namespace dbshell;
using std::stringstream;
using std::string;
using std::cout;
using std::endl;
using std::ostream;
using std::shared_ptr;

double_column::double_column(string name) : column(name, alignment_type::LEFT) {
}

double_column::~double_column() {
}

string double_column::format(double value) const {
  stringstream s;
  s << value;
  return s.str();
}

string double_column::str() const {
  return "double_column(" + name() + ")";
}
