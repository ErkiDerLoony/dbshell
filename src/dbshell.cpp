#include "postgres_adapter.hpp"
#include "readline_adapter.hpp"
#include "formatter.hpp"
#include "getopt.hpp"
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
  std::shared_ptr<db_adapter> connection;
  list<std::unique_ptr<db_adapter>> connections;

  { // Release getopt instance asap.
    getopt args(argc, argv);

    if (args.list().empty()) {
      cerr << "You must specify at least one connection string on the command line!" << endl;
      return 1;
    }

    // for (string entry : args.list()) {
    string entry = args.list().front();
    auto index = entry.find("://");

    if (index == string::npos) {
      cerr << "Could not find schema in connection string »" << entry << "«!" << endl;
      return 1;
    }

    string schema = entry.substr(0, index);
    string rest = entry.substr(index + 3, entry.length());

    if (rest == "") {
      cerr << "No hostname in connection string »" << entry << "«!" << endl;
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

    if (args.list().size() > 1) {
      cout << "WARNING: Only one connection is supported. Ignoring further connections." << endl;
    }

    // }
  }

  return 0;

  postgres_adapter db("dwh.camato.eu", "camato", "dwh_production");

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
      pair<vector<string>, vector<vector<string>>> table = db.query(line);
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
