#include "table.hpp"

#include <memory>
#include <vector>
#include <sstream>
#include <iostream>
#include <tuple>

using namespace dbshell;
using std::initializer_list;
using std::stringstream;
using std::runtime_error;
using std::range_error;
using std::cout;
using std::endl;
using std::unique_ptr;
using std::shared_ptr;
using std::string;
using std::vector;
using std::ostream;
using std::tuple;
using std::pair;
using std::get;

table::table() {
}

table::~table() {
}

uint table::columns() const {
  return _columns.size();
}

const string& table::column(uint index) const throw(range_error) {

  if (index >= columns()) {
    stringstream buffer;
    buffer << "No column with index " << index << "!";
    throw range_error(buffer.str());
  }

  return _columns[index];
}

uint table::rows() const {
  return _rows.size();
}

const vector<string>& table::row(const uint index) const throw(range_error) {

  if (index >= rows()) {
    stringstream buffer;
    buffer << "No row with index " << index << "!";
    throw range_error(buffer.str());
  }

  return _rows[index];
}

const vector<string>& table::operator[](const uint index) const throw(range_error) {

  if (index >= rows()) {
    stringstream buffer;
    buffer << "No row with index " << index << "!";
    throw range_error(buffer.str());
  }

  return _rows[index];
}

const alignment_type& table::alignment(const uint index) const throw(range_error) {

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

void table::add(vector<string> row) throw(runtime_error) {

  if (row.size() != _columns.size()) {
    throw runtime_error("Wrong number of entries for this table!");
  }

  _rows.push_back(row);
}

void table::add(initializer_list<string> row) throw(runtime_error) {

  if (row.size() != _columns.size()) {
    throw runtime_error("Wrong number of entries for this table!");
  }

  _rows.push_back(vector<string>(row));
}

const vector<vector<string>>::const_iterator table::begin() const {
  return _rows.begin();
}

const vector<vector<string>>::const_iterator table::end() const {
  return _rows.end();
}

struct size_type {
  uint pre;
  uint post;
  uint sum;
};

shared_ptr<vector<size_type>> sizes(const table& table) {
  const shared_ptr<vector<size_type>> sizes = shared_ptr<vector<size_type>>(new vector<size_type>());

  for (uint col = 0; col < table.columns(); col++) {
    size_type t;
    t.pre = 0;
    t.post = 0;
    t.sum = 0;
    sizes->push_back(t);
  }

  for (vector<string> row : table) {

    for (uint col = 0; col < row.size(); col++) {
      const string anchor = table.alignment_string(col);
      auto pre = row[col].find(anchor);

      if (pre == string::npos) {
        continue;
      }

      uint post = row[col].length() - row[col].find(anchor) - anchor.length();

      if (pre > (*sizes)[col].pre) {
        (*sizes)[col].pre = pre;
      }

      if (post > (*sizes)[col].post) {
        (*sizes)[col].post = post;
      }
    }
  }

  for (uint col = 0; col < table.columns(); col++) {
    (*sizes)[col].sum = (*sizes)[col].pre + (*sizes)[col].post + table.alignment_string(col).length();
  }

  return sizes;
}

void format_header(ostream& stream, const table& table,
                   const shared_ptr<vector<size_type>> sizes) {

  for (uint i = 0; i < table.columns(); i++) {
    stream << "│ ";
    size_type size = (*sizes)[i];
    string column = table.column(i);

    for (uint k = 0; size.sum > column.length() && k < (size.sum - column.length()) / 2; k++) {
      stream << " ";
    }

    stream << column << " ";

    for (uint k = 0; size.sum > column.length() && k < (size.sum - column.length() + 1) / 2; k++) {
      stream << " ";
    }
  }

  stream << "│" << endl;
}

void format_rows(ostream& stream, const table& table,
                 const shared_ptr<vector<size_type>> sizes) {

  for (uint i = 0; i < table.rows(); i++) {
    vector<string> row = table.row(i);

    for (uint j = 0; j < row.size(); j++) {
      size_type size = (*sizes)[j];
      string anchor = table.alignment_string(j);
      string header = table.column(j);
      stream << "│ ";

      if (header.length() > size.sum) {
        switch (table.alignment(j)) {
        case alignment_type::LEFT:
          break;
        case alignment_type::CENTER:
          for (uint k = 0; k < (header.length() - size.sum) / 2; k++)
            stream << " ";
          break;
        case alignment_type::RIGHT:
          for (uint k = 0; k < header.length() - size.sum; k++)
            stream << " ";
          break;
        }
      }

      if (row[j].find(anchor) != string::npos) {
        if (anchor != "") {
          for (uint k = 0; k < size.pre - row[j].find(anchor); k++)
            stream << " ";
        } else {
          switch (table.alignment(j)) {
          case alignment_type::LEFT:
            break;
          case alignment_type::CENTER:
            for (uint k = 0; k < (size.sum - row[j].length()) / 2; k++)
              stream << " ";
            break;
          case alignment_type::RIGHT:
            for (uint k = 0; k < size.sum - row[j].length(); k++)
              stream << " ";
            break;
          }
        }
      } else {
        switch (table.alignment(j)) {
        case alignment_type::LEFT:
          break;
        case alignment_type::CENTER:
          for (uint k = 0; k < (size.sum - row[j].length()) / 2; k++)
            stream << " ";
          break;
        case alignment_type::RIGHT:
          for (uint k = 0; k < size.sum - row[j].length(); k++)
            stream << " ";
          break;
        }
      }

      stream << row[j];

      if (header.length() > size.sum) {
        switch (table.alignment(j)) {
        case alignment_type::RIGHT:
          break;
        case alignment_type::CENTER:
          for (uint k = 0; k < (header.length() - size.sum + 1) / 2; k++)
            stream << " ";
          break;
        case alignment_type::LEFT:
          for (uint k = 0; k < header.length() - size.sum; k++)
            stream << " ";
          break;
        }
      }

      if (row[j].find(anchor) != string::npos) {
        if (anchor != "") {
          for (uint k = 0; k < 1 + size.post - (row[j].length() - row[j].find(anchor) - anchor.length()); k++)
            stream << " ";
        } else {
          switch (table.alignment(j)) {
          case alignment_type::LEFT:
            for (uint k = 0; k < 1 + size.sum - row[j].length(); k++)
              stream << " ";
            break;
          case alignment_type::CENTER:
            for (uint k = 0; k < 1 + (size.sum - row[j].length() + 1) / 2; k++)
              stream << " ";
            break;
          case alignment_type::RIGHT:
            stream << " ";
            break;
          }
        }
      } else {
        switch (table.alignment(j)) {
        case alignment_type::RIGHT:
          stream << " ";
          break;
        case alignment_type::CENTER:
          for (uint k = 0; k < 1 + (size.sum - row[j].length() + 1) / 2; k++)
            stream << " ";
          break;
        case alignment_type::LEFT:
          for (uint k = 0; k < 1 + size.sum - row[j].length(); k++)
            stream << " ";
          break;
        }
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

  shared_ptr<vector<size_type>> sizes = ::sizes(table);

  stream << "sizes = [";

  for (uint i = 0; i < sizes->size(); i++) {
    stream << "(" << (*sizes)[i].pre << ", " << (*sizes)[i].post << ", " << (*sizes)[i].sum << ")";

    if (i < sizes->size() - 1) {
      stream << ", ";
    } else {
      stream << "]" << endl;
    }
  }

  stream << "┌";

  for (uint i = 0; i < sizes->size(); i++) {

    for (uint k = 0; k < (*sizes)[i].sum + 2 || k < table.column(i).length() + 2; k++) {
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

    for (uint k = 0; k < (*sizes)[i].sum + 2 || k < table.column(i).length() + 2; k++) {
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

    for (uint k = 0; k < (*sizes)[i].sum + 2 || k < table.column(i).length() + 2; k++) {
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
