/*
 * © Copyright 2013 by Edgar Kalkowski <eMail@edgar-kalkowski.de>
 * 
 * This file is part of the program dbshell.
 * 
 * The program dbshell is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
using std::wcout;
using std::cout;
using std::endl;
using std::unique_ptr;
using std::shared_ptr;
using std::wstring;
using std::string;
using std::vector;
using std::wostream;
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

uint table::cols() const {
  return columns();
}

const wstring& table::column(uint index) const throw(range_error) {

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

const vector<wstring>& table::row(const uint index) const throw(range_error) {

  if (index >= rows()) {
    stringstream buffer;
    buffer << "No row with index " << index << "!";
    throw range_error(buffer.str());
  }

  return _rows[index];
}

const vector<wstring>& table::operator[](const uint index) const throw(range_error) {

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

const wstring& table::alignment_string(uint index) const throw(range_error) {

  if (index >= columns()) {
    stringstream buffer;
    buffer << "No column with index " << index << "!";
    throw range_error(buffer.str());
  }

  return _alignment_strings[index];
}

void table::add_column(wstring name, alignment_type alignment,
                       wstring alignment_string) {
  add(name, alignment, alignment_string);
}

void table::add(wstring name, alignment_type alignment,
                wstring alignment_string) {
  _columns.push_back(name);
  _alignments.push_back(alignment);
  _alignment_strings.push_back(alignment_string);
}

void table::add(vector<wstring> row) throw(runtime_error) {

  if (row.size() != _columns.size()) {
    stringstream buffer;
    buffer << "Wrong number of entries for this table (" << row.size()
           << " entrie(s) vs. " << _columns.size() << " column(s))!";
    throw runtime_error(buffer.str());
  }

  _rows.push_back(row);
}

void table::add(initializer_list<wstring> row) throw(runtime_error) {

  if (row.size() != _columns.size()) {
    stringstream buffer;
    buffer << "Wrong number of entries for this table (" << row.size()
           << " entrie(s) vs. " << _columns.size() << " column(s))!";
    throw runtime_error(buffer.str());
  }

  _rows.push_back(vector<wstring>(row));
}

wstring table::get(const uint row, const uint col) const throw(range_error) {

  if (row < 0 || row > _rows.size() || col < 0 || col > _columns.size()) {
    throw range_error("Invalid indices!");
  }

  return _rows[row][col];
}

void table::set(const uint row, const uint col, const wstring value) throw(range_error) {

  if (row < 0 || row > _rows.size() || col < 0 || col > _columns.size()) {
    throw range_error("Invalid indices!");
  }

  _rows[row][col] = value;
}

const vector<vector<wstring>>::const_iterator table::begin() const {
  return _rows.begin();
}

const vector<vector<wstring>>::const_iterator table::end() const {
  return _rows.end();
}

struct size_type {
  uint pre;
  uint post;
  uint sum;
};

unique_ptr<vector<size_type>> sizes(const table& table) {
  unique_ptr<vector<size_type>> sizes = unique_ptr<vector<size_type>>(new vector<size_type>());
  vector<bool> contains_alignment_string;

  for (uint col = 0; col < table.columns(); col++) {
    size_type t;
    t.pre = 0;
    t.post = 0;
    t.sum = 0;
    sizes->push_back(t);

    contains_alignment_string.push_back(false);
  }

  for (vector<wstring> row : table) {

    for (uint col = 0; col < row.size(); col++) {
      const wstring anchor = table.alignment_string(col);
      auto pre = row[col].find(anchor);
      uint post;

      if (pre == string::npos) {
        pre = row[col].length();
        post = 0;
      } else {
        contains_alignment_string[col] = true;
        post = row[col].length() - row[col].find(anchor) - anchor.length();
      }

      if (pre > (*sizes)[col].pre) {
        (*sizes)[col].pre = pre;
      }

      if (post > (*sizes)[col].post) {
        (*sizes)[col].post = post;
      }
    }
  }

  for (uint col = 0; col < table.columns(); col++) {
    (*sizes)[col].sum = (*sizes)[col].pre + (*sizes)[col].post;

    if (contains_alignment_string[col]) {
      (*sizes)[col].sum += table.alignment_string(col).length();
    }
  }

  return sizes;
}

void format_header(wostream& stream, const table& table,
                   const vector<size_type>& sizes) {

  for (uint i = 0; i < table.columns(); i++) {
    stream << L"│ ";
    size_type size = sizes[i];
    wstring column = table.column(i);

    for (uint k = 0; size.sum > column.length() && k < (size.sum - column.length()) / 2; k++) {
      stream << L" ";
    }

    stream << column << L" ";

    for (uint k = 0; size.sum > column.length() && k < (size.sum - column.length() + 1) / 2; k++) {
      stream << L" ";
    }
  }

  stream << L"│" << endl;
}

void format_rows(wostream& stream, const table& table,
                 const vector<size_type>& sizes) {

  for (uint i = 0; i < table.rows(); i++) {
    vector<wstring> row = table.row(i);

    for (uint j = 0; j < row.size(); j++) {
      size_type size = sizes[j];
      wstring anchor = table.alignment_string(j);
      wstring header = table.column(j);
      stream << L"│ ";

      if (header.length() > size.sum) {
        switch (table.alignment(j)) {
        case alignment_type::LEFT:
          break;
        case alignment_type::CENTER:
          for (uint k = 0; k < (header.length() - size.sum) / 2; k++)
            stream << L" ";
          break;
        case alignment_type::RIGHT:
          for (uint k = 0; k < header.length() - size.sum; k++)
            stream << L" ";
          break;
        }
      }

      if (row[j].find(anchor) != string::npos) {
        if (anchor != L"") {
          for (uint k = 0; k < size.pre - row[j].find(anchor); k++)
            stream << L" ";
        } else {
          switch (table.alignment(j)) {
          case alignment_type::LEFT:
            break;
          case alignment_type::CENTER:
            for (uint k = 0; k < (size.sum - row[j].length()) / 2; k++)
              stream << L" ";
            break;
          case alignment_type::RIGHT:
            for (uint k = 0; k < size.sum - row[j].length(); k++)
              stream << L" ";
            break;
          }
        }
      } else {
        for (uint k = 0; k < size.pre - row[j].length(); k++)
          stream << L" ";
      }

      stream << row[j];

      if (header.length() > size.sum) {
        switch (table.alignment(j)) {
        case alignment_type::RIGHT:
          break;
        case alignment_type::CENTER:
          for (uint k = 0; k < (header.length() - size.sum + 1) / 2; k++)
            stream << L" ";
          break;
        case alignment_type::LEFT:
          for (uint k = 0; k < header.length() - size.sum; k++)
            stream << L" ";
          break;
        }
      }

      if (row[j].find(anchor) != string::npos) {
        if (anchor != L"") {
          for (uint k = 0; k < 1 + size.post - (row[j].length() - row[j].find(anchor) - anchor.length()); k++)
            stream << L" ";
        } else {
          switch (table.alignment(j)) {
          case alignment_type::LEFT:
            for (uint k = 0; k < 1 + size.sum - row[j].length(); k++)
              stream << L" ";
            break;
          case alignment_type::CENTER:
            for (uint k = 0; k < 1 + (size.sum - row[j].length() + 1) / 2; k++)
              stream << L" ";
            break;
          case alignment_type::RIGHT:
            stream << L" ";
            break;
          }
        }
      } else {
        for (uint k = 0; k < 1 + size.sum - size.pre; k++)
          stream << L" ";
      }
    }

    stream << L"│" << endl;
  }
}

wostream& operator<<(wostream& stream, const table& table) {

  if (table.columns() == 0) {
    stream << L"┌─┐" << endl << L"│\u2205│" << endl << L"└─┘";
    return stream;
  }

  unique_ptr<vector<size_type>> sizes = ::sizes(table);

  /*
  stream << "sizes = [";

  for (uint i = 0; i < sizes->size(); i++) {
    stream << "(" << (*sizes)[i].pre << ", " << (*sizes)[i].post << ", " << (*sizes)[i].sum << ")";

    if (i < sizes->size() - 1) {
      stream << ", ";
    } else {
      stream << "]" << endl;
    }
  }
  */

  stream << L"┌";

  for (uint i = 0; i < sizes->size(); i++) {

    for (uint k = 0; k < (*sizes)[i].sum + 2 || k < table.column(i).length() + 2; k++) {
      stream << L"─";
    }

    if (i < sizes->size() - 1) {
      stream << L"┬";
    } else {
      stream << L"┐" << endl;
    }
  }

  format_header(stream, table, *sizes);

  stream << L"├";

  for (uint i = 0; i < sizes->size(); i++) {

    for (uint k = 0; k < (*sizes)[i].sum + 2 || k < table.column(i).length() + 2; k++) {
      stream << L"─";
    }

    if (i < sizes->size() - 1) {
      stream << L"┼";
    } else {
      stream << L"┤" << endl;
    }
  }

  format_rows(stream, table, *sizes);

  stream << L"└";

  for (uint i = 0; i < sizes->size(); i++) {

    for (uint k = 0; k < (*sizes)[i].sum + 2 || k < table.column(i).length() + 2; k++) {
      stream << L"─";
    }

    if (i < sizes->size() - 1) {
      stream << L"┴";
    } else {
      stream << L"┘";
    }
  }

  return stream;
}
