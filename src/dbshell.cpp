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

#include "readline_adapter.hpp"
#include "formatter.hpp"
#include "db_adapter.hpp"
#include "virtuoso_adapter.hpp"
#include "postgres_adapter.hpp"
#include "table.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <memory>
#include <vector>
#include <locale>

#include <signal.h>
#include <sys/time.h>

using namespace dbshell;
using std::unique_ptr;
using std::string;
using std::wstring;
using std::wstringstream;
using std::move;
using std::wcout;
using std::cerr;
using std::cout;
using std::endl;
using std::flush;
using std::pair;
using std::stringstream;
using std::runtime_error;
using std::vector;

namespace dbshell {
  bool running = false;
  bool transpose = false;
  unique_ptr<db_adapter> connection = unique_ptr<db_adapter>(nullptr);
}

void handler(int sig) {

  dbshell::clear_input();

  if (!running) {
    return;
  }

  dbshell::connection->cancel();
}

unique_ptr<dbshell::table> transpose(unique_ptr<table> table) {
  unique_ptr<dbshell::table> copy = unique_ptr<dbshell::table>(new dbshell::table());
  copy->add_column(L"Column");

  for (uint i = 0; i < table->rows(); i++) {
    wstringstream name;
    name << L"Row № " << (i+1);
    copy->add(name.str());
  }

  for (uint i = 0; i < table->cols(); i++) {
    vector<wstring> row;
    row.push_back(table->column(i));

    for (uint j = 0; j < table->rows(); j++) {
      row.push_back(table->row(j)[i]);
    }

    copy->add(row);
  }

  return copy;
}

int main(int argc, char** argv) {

  // Fix io of UTF-8 characters.
  std::ios_base::sync_with_stdio(false);
  std::wcout.imbue(std::locale(""));

  if (argc < 2) {
    cerr << "You must specify a connection string on the command line!" << endl;
    return 1;
  }

  if (string(argv[1]) == "--help" || string(argv[1]) == "-h" || string(argv[1]) == "-help") {
    cout << "Usage: dbshell <schema>://[<username>@]<dbhost>[/dbname]" << endl;
    return 0;
  }

  if (string(argv[1]) == "--version" || string(argv[1]) == "-version") {
    cout << "dbshell version 0.1.0" << endl;
    return 0;
  }

  string arg(argv[1]);
  auto index = arg.find("://");

  if (index == string::npos) {
    cerr << "Could not find schema in connection string »" << arg << "«!" << endl;
    return 1;
  }

  string schema = arg.substr(0, index);
  string rest = arg.substr(index + 3, arg.length());

  if (rest == "") {
    cerr << "No hostname in connection string »" << arg << "«!" << endl;
    return 1;
  }

  string username = "";
  index = rest.find("@");

  if (index != string::npos) {
    username = rest.substr(0, index);
    rest = rest.substr(index + 1, rest.length());
  }

  string host = "";
  index = rest.find("/");

  if (index != string::npos) {
    host = rest.substr(0, index);
    rest = rest.substr(index + 1, rest.length());
  } else {
    host = rest;
  }

  string database = rest;

  if (schema == "psql" || schema == "postgres" || schema == "postgresql") {
    dbshell::connection = unique_ptr<db_adapter>(new postgres_adapter(host, username, database));
  } else if (schema == "virtuoso") {
    dbshell::connection = unique_ptr<db_adapter>(new virtuoso_adapter(host, username));
  } else {
    cerr << "Unsupported database type: " << schema << endl;
    return 1;
  }

  signal(SIGABRT, &handler);
  signal(SIGTERM, &handler);
  signal(SIGINT, &handler);

  prompt = "-> ";
  string line;

  while (true) {
    line = readline();

    if (line.substr(0, 1) != "\\") {
      line = line.substr(0, line.length() - 1);
    } else {

      if (line.substr(line.length() - 1, line.length()) == ";") {
        line = line.substr(0, line.length() - 1);
      }
    }

    if (line == "quit" || line == "\\q") {
      break;
    }

    if (line == "\\x") {

      if (dbshell::transpose) {
        wcout << L"No longer transposing output tables." << endl;
        dbshell::transpose = false;
      } else {
        wcout << L"Transposing future output tables." << endl;
        dbshell::transpose = true;
      }

      continue;
    }

    try {
      struct timeval start;
      gettimeofday(&start, nullptr);
      dbshell::running = true;
      unique_ptr<table> table = dbshell::connection->query(line);
      dbshell::running = false;

      if (table == nullptr) {
	continue;
      }

      struct timeval end;
      gettimeofday(&end, nullptr);
      const long diff = end.tv_sec * 1000 + end.tv_usec / 1000 - start.tv_sec * 1000 - start.tv_usec / 1000;

      if (dbshell::transpose) {
        table = ::transpose(move(table));
      }

      wcout << *table << endl;
      wcout << table->rows() << " row";
      if (table->rows() != 1) wcout << "s";
      wcout << L", "
            << wstring(format_duration(diff).c_str())
            << endl;
    } catch (runtime_error e) {
      wcout << e.what() << flush;
    }
  }

  wcout << "Bye." << endl;

  return 0;
}
