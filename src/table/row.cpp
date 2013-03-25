#include "row.hpp"

using namespace dbshell;
using std::shared_ptr;
using std::ostream;

row::row() {
}

row::~row() {
}

void row::set(shared_ptr<column> column, std::shared_ptr<void> value) {

}

ostream& operator<<(ostream& stream, const row& row) {
  stream << "row";
  return stream;
}
