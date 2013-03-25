#include "long_column.hpp"

#include <sstream>

using namespace dbshell;
using std::stringstream;
using std::string;

long_column::long_column(std::string name) : column(name), _alignment(alignment_type::LEFT) {
}

long_column::~long_column() {
}

string long_column::name() const {
  return column::name();
}

string long_column::format(long value) const {
  stringstream s;
  s << value;
  return s.str();
}

alignment_type long_column::alignment() const {
  return _alignment;
}
