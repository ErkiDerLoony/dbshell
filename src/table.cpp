#include "table.hpp"

#include <memory>
#include <vector>
#include <sstream>

using namespace dbshell;
using std::stringstream;
using std::runtime_error;
using std::range_error;
using std::endl;
using std::unique_ptr;
using std::shared_ptr;
using std::string;
using std::vector;
using std::ostream;
using std::tuple;
using std::get;

table::table() {
}

table::~table() {
}

uint table::columns() const {
  return _columns.size();
}

uint table::rows() const {
  return _rows.size();
}

const string& table::column(uint index) const throw(range_error) {

  if (index >= columns()) {
    stringstream buffer;
    buffer << "No column with index " << index << "!";
    throw range_error(buffer.str());
  }

  return _columns[index];
}

const alignment_type& table::alignment(uint index) const throw(range_error) {

  if (index >= columns()) {
    stringstream buffer;
    buffer << "No column with index " << index << "!";
    throw range_error(buffer.str());
  }

  return _alignments[index];
}

const string& table::alignment_string(uint index) const throw(range_error) {

  if (index >= columns()) {
    stringstream buffer;
    buffer << "No column with index " << index << "!";
    throw range_error(buffer.str());
  }

  return _alignment_strings[index];
}

void table::add(string name, alignment_type alignment,
                string alignment_string) {
  _columns.push_back(name);
  _alignments.push_back(alignment);
  _alignment_strings.push_back(alignment_string);
}

const vector<string>& table::row(uint index) const throw(range_error) {

  if (index >= rows()) {
    stringstream buffer;
    buffer << "No row with index " << index << "!";
    throw range_error(buffer.str());
  }

  return _rows[index];
}

void table::add(vector<string> row) throw(runtime_error) {

  if (row.size() != _columns.size()) {
    throw runtime_error("Wrong number of entries for this table!");
  }

  _rows.push_back(row);
}

shared_ptr<vector<uint>> size(const table& table) {
  shared_ptr<vector<uint>> sizes = shared_ptr<vector<uint>>(new vector<uint>());

  for (uint i = 0; i < table.columns(); i++) {
    sizes->push_back(table.column(i).length());
  }

  for (uint i = 0; i < table.rows(); i++) {
    vector<string> row = table.row(i);

    for (uint k = 0; k < row.size(); k++) {
      const uint size = row[k].size();

      if (size > (*sizes)[k]) {
        (*sizes)[k] = size;
      }
    }
  }

  return sizes;
}

void format_header(ostream& stream, const table& table, const shared_ptr<vector<uint>> sizes) {

  for (uint i = 0; i < table.columns(); i++) {
    stream << "│ ";
    string column = table.column(i);

    for (uint k = 0; k < ((*sizes)[i] - column.length()) / 2; k++) {
      stream << " ";
    }

    stream << column << " ";

    for (uint k = 0; k < ((*sizes)[i] - column.length() + 1) / 2; k++) {
      stream << " ";
    }
  }

  stream << "│" << endl;
}

void format_rows(ostream& stream, const table& table, const shared_ptr<vector<uint>> sizes) {

  for (uint i = 0; i < table.rows(); i++) {
    vector<string> row = table.row(i);

    for (uint j = 0; j < row.size(); j++) {
      stream << "│";

      switch (table.alignment(j)) {
      case alignment_type::LEFT:
        stream << " ";
        break;
      case alignment_type::CENTER:

        for (uint k = 0; k < 1 + ((*sizes)[j] - row[j].length() + 1) / 2; k++) {
          stream << " ";
        }

        break;
      case alignment_type::RIGHT:

        for (uint k = 0; k < (*sizes)[j] - row[j].length() + 1; k++) {
          stream << " ";
        }

        break;
      }

      stream << row[j];

      switch (table.alignment(j)) {
      case alignment_type::LEFT:

        for (uint k = 0; k < (*sizes)[j] - row[j].length() + 1; k++) {
          stream << " ";
        }

        break;
      case alignment_type::CENTER:

        for (uint k = 0; k < 1 + ((*sizes)[j] - row[j].length()) / 2; k++) {
          stream << " ";
        }

        break;
      case alignment_type::RIGHT:
        stream << " ";
        break;
      }
    }

    stream << "│" << endl;
  }
}

ostream& operator<<(ostream& stream, const table& table) {

  if (table.columns() == 0) {
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

  format_rows(stream, table, sizes);

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
