#include "column.hpp"

using namespace dbshell;
using std::string;
using std::ostream;
using std::shared_ptr;

column::column(string name, alignment_type alignment) : _name(name), _alignment(alignment) {
}

column::~column() {
}

string column::name() const {
  return _name;
}

alignment_type column::alignment() const {
  return _alignment;
}

ostream& operator<<(ostream& stream, column const& column) {
  stream << column.str();
  return stream;
}
