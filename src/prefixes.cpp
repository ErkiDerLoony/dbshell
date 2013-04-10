#include "prefixes.hpp"

#include <iostream>
#include <sstream>
#include <vector>

using namespace dbshell;
using std::wstringstream;
using std::wstring;
using std::stringstream;
using std::string;
using std::pair;
using std::vector;
using std::unique_ptr;

prefixes::prefixes() {
  _prefixes.insert(pair<string, string>("rdf:", "http://www.w3.org/1999/02/22-rdf-syntax-ns#"));
  _prefixes.insert(pair<string, string>("rdfs:", "http://www.w3.org/2000/01/rdf-schema#"));
  _prefixes.insert(pair<string, string>("xsd:", "http://www.w3.org/2001/XMLSchema#"));
  _prefixes.insert(pair<string, string>("dc:", "http://purl.org/dc/terms/"));
  _prefixes.insert(pair<string, string>("owl:", "http://www.w3.org/2002/07/owl#"));
  _prefixes.insert(pair<string, string>("dbpedia:", "http://dbpedia.org/"));
  _prefixes.insert(pair<string, string>("de.dbpedia:", "http://de.dbpedia.org/"));
}

prefixes::~prefixes() {
}

void prefixes::prefix(dbshell::table& t) const {

  for (uint row = 0; row < t.rows(); row++) {

    for (uint col = 0; col < t.columns(); col++) {

      for (pair<string, string> prefix : _prefixes) {
        wstringstream buffer;
        buffer << "<" << prefix.second.c_str();
        wstring value = t.get(row, col);

        if (value.substr(0, prefix.second.length() + 1) == buffer.str()) {
          wstringstream updated;
          updated << prefix.first.c_str()
                  << value.substr(prefix.second.length() + 1,
                                  value.length() - prefix.second.length() - 2);
          t.set(row, col, updated.str());
          break;
        }
      }
    }
  }
}

string prefixes::prefix(const string& t) const {
  std::cout << "Prefixing »" << t << "«." << std::endl;

  for (pair<string, string> prefix : _prefixes) {

    if (t.substr(0, prefix.second.length()) == prefix.second) {
      stringstream updated;
      updated << prefix.first.c_str()
              << t.substr(prefix.second.length(),
                          t.length() - prefix.second.length());
      return updated.str();
    }
  }

  return t;
}

string prefixes::format() const {
  stringstream result;
  bool first = true;

  for (pair<string, string> prefix : _prefixes) {

    if (first) {
      first = false;
    } else {
      result << " ";
    }

    result << "PREFIX " << prefix.first << " <" << prefix.second << ">";
  }

  return result.str();
}

unique_ptr<table> prefixes::format_as_table() const {
  unique_ptr<table> result = unique_ptr<table>(new table());

  result->add(L"Prefix");
  result->add(L"URI");

  for (pair<string, string> prefix : _prefixes) {
    wstringstream first;
    first << prefix.first.c_str();

    wstringstream second;
    second << prefix.second.c_str();

    vector<wstring> row;
    row.push_back(first.str());
    row.push_back(second.str());
    result->add(row);
  }

  return result;
}
