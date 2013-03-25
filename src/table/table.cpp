#include "table.hpp"
#include "double_column.hpp"
#include "long_column.hpp"
#include "row.hpp"

#include <iostream>
#include <sstream>

using namespace dbshell;
using std::stringstream;
using std::string;
using std::shared_ptr;
using std::vector;
using std::cout;
using std::endl;
using std::ostream;

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

void table::add_row(shared_ptr<row> row) {
  _rows.push_back(row);
  if (table::debug) cout << "Row " << (*row) << " added." << endl;
}

const vector<shared_ptr<column>>& table::columns() const {
  return _columns;
}

shared_ptr<vector<uint>> size(const table& table) {
  shared_ptr<vector<uint>> sizes = shared_ptr<vector<uint>>(new vector<uint>());

  for (std::shared_ptr<column> column : table.columns()) {
    sizes->push_back(column->name().length());
  }

  /*
  for (vector<string> row : rows) {

    for (uint i = 0; i < header.size(); i++) {
      const uint size = row[i].size();

      if (size > sizes[i]) {
        sizes[i] = size;
      }
    }
  }
  */

  return sizes;
}

void format_header(ostream& stream, const table& table, const shared_ptr<vector<uint>> sizes) {
  vector<shared_ptr<column>> columns = table.columns();

  for (uint i = 0; i < columns.size(); i++) {
    stream << "│ ";

    for (uint k = 0; k < ((*sizes)[i] - columns[i]->name().length()) / 2; k++) {
      stream << " ";
    }

    stream << columns[i]->name() << " ";

    for (uint k = 0; k < ((*sizes)[i] - columns[i]->name().length() + 1) / 2; k++) {
      stream << " ";
    }
  }

  stream << "│" << endl;
}

ostream& operator<<(ostream& stream, const table& table) {

  if (table.columns().size() == 0) {
    stream << "┌─┐" << endl << "│\u2205│" << endl << "└─┘";
    return stream;
  }

  shared_ptr<vector<uint>> sizes = size(table);
  stream << "┌";

  for (uint i = 0; i < sizes->size(); i++) {

    for (uint k = 0; k < (*sizes)[i] + 2; k++) {
      stream << "─";
    }

    if (i < sizes->size() - 1) {
      stream << "┬";
    } else {
      stream << "┐" << endl;
    }
  }

  format_header(stream, table, sizes);

  stream << "├";

  for (uint i = 0; i < sizes->size(); i++) {

    for (uint k = 0; k < (*sizes)[i] + 2; k++) {
      stream << "─";
    }

    if (i < sizes->size() - 1) {
      stream << "┼";
    } else {
      stream << "┤" << endl;
    }
  }

  //format_rows(rows, sizes, stream);

  stream << "└";

  for (uint i = 0; i < sizes->size(); i++) {

    for (uint k = 0; k < (*sizes)[i] + 2; k++) {
      stream << "─";
    }

    if (i < sizes->size() - 1) {
      stream << "┴";
    } else {
      stream << "┘";
    }
  }

  return stream;
}

int main(int argc, char** argv) {
  table::debug = true;
  shared_ptr<long_column> timestamp = shared_ptr<long_column>(new long_column("timestamp"));
  shared_ptr<double_column> value = shared_ptr<double_column>(new double_column("value"));

  table table;
  table.add_column(timestamp);
  table.add_column(value);

  shared_ptr<row> r = shared_ptr<row>(new row());
  r->set(timestamp, shared_ptr<int>(new int(2)));
  r->set(value, shared_ptr<double>(new double(0.5)));
  table.add_row(r);

  cout << table << endl;

  return 0;
}
