#include "prefixes.hpp"

#include <iostream>
#include <sstream>
#include <vector>

using namespace dbshell;
using std::wstringstream;
using std::wstring;
using std::pair;
using std::vector;
using std::unique_ptr;

prefixes::prefixes() {
  _prefixes.insert(pair<wstring, wstring>(L"rdf:", L"http://www.w3.org/1999/02/22-rdf-syntax-ns#"));
}

prefixes::~prefixes() {
}

void prefixes::prefix(dbshell::table& t) const {

  for (uint row = 0; row < t.rows(); row++) {

    for (uint col = 0; col < t.columns(); col++) {

      for (pair<wstring, wstring> prefix : _prefixes) {
        wstringstream buffer;
        buffer << "<" << prefix.second;
        wstring value = t.get(row, col);

        if (value.substr(0, prefix.second.length() + 1) == buffer.str()) {
          wstringstream updated;
          updated << prefix.first << value.substr(prefix.second.length() + 1, value.length() - prefix.second.length() - 2);
          t.set(row, col, updated.str());
          break;
        }
      }
    }
  }
}

wstring prefixes::format() const {
  wstringstream result;
  bool first = true;

  for (pair<wstring, wstring> prefix : _prefixes) {

    if (first) {
      first = false;
    } else {
      result << " ";
    }

    result << prefix.first << " " << prefix.second;
  }

  return result.str();
}

unique_ptr<table> prefixes::format_as_table() const {
  unique_ptr<table> result = unique_ptr<table>(new table());

  result->add(L"Prefix");
  result->add(L"URI");

  for (pair<wstring, wstring> prefix : _prefixes) {
    vector<wstring> row;
    row.push_back(prefix.first);
    row.push_back(prefix.second);
    result->add(row);
  }

  return result;
}
