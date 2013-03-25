#include "double_column.hpp"

#include <sstream>
#include <iostream>

using namespace dbshell;
using std::stringstream;
using std::string;
using std::cout;
using std::endl;

double_column::double_column(string name) : column(name), _alignment(alignment_type::LEFT) {
}

double_column::~double_column() {
}

string double_column::name() const {
  return column::name();
}

string double_column::format(double value) const {
  stringstream s;
  s << value;
  return s.str();
}

alignment_type double_column::alignment() const {
  return _alignment;
}
