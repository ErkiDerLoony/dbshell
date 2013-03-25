#include "column.hpp"

using namespace dbshell;
using std::string;

column::column(string name) : _name(name) {
}

column::~column() {
}

string column::name() const {
  return _name;
}
