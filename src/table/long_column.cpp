#include "long_column.hpp"

#include <sstream>

using namespace dbshell;
using std::stringstream;
using std::string;

long_column::long_column(std::string name) : column(name, alignment_type::LEFT) {
}

long_column::~long_column() {
}

string long_column::format(long value) const {
  stringstream s;
  s << value;
  return s.str();
}

string long_column::str() const {
  return "long_column(" + name() + ")";
}
