#include "postgres_adapter.hpp"
#include "readline_adapter.hpp"
#include "formatter.hpp"
#include "db_adapter.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <stdexcept>
#include <memory>

#include <sys/time.h>

using namespace dbshell;
using std::vector;
using std::list;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::pair;
using std::stringstream;
using std::runtime_error;

int main(int argc, char** argv) {

  if (argc < 2) {
    cerr << "You must specify a connection string on the command line!" << endl;
    return 1;
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
  }

  string database = rest;
  std::shared_ptr<db_adapter> db;

  if (schema == "postgres" || schema == "postgresql") {
    db = std::shared_ptr<db_adapter>(new postgres_adapter(host, username, database));
  } else {
    cerr << "Unsupported database type: " << schema << endl;
    return 1;
  }

  prompt = "-> ";
  string line;

  while (true) {
    line = readline();

    if (line == "quit" || line == "quit;") {
      break;
    }

    try {
      struct timeval start;
      gettimeofday(&start, nullptr);
      pair<vector<string>, vector<vector<string>>> table = db->query(line);
      struct timeval end;
      gettimeofday(&end, nullptr);
      const long diff = end.tv_sec * 1000 + end.tv_usec / 1000 - start.tv_sec * 1000 - start.tv_usec / 1000;
      cout << format(table.first, table.second) << endl;
      cout << table.second.size() << " row";
      if (table.second.size() != 1) cout << "s";
      cout << ", " << format_duration(diff) << endl;
    } catch (runtime_error e) {
      cout << e.what();
    }
  }

  cout << "Bye." << endl;

  return 0;
}
