#include "postgres_adapter.hpp"
#include "readline_adapter.hpp"
#include "formatter.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

#include <sys/time.h>

using namespace dbshell;
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::pair;
using std::stringstream;
using std::runtime_error;

int main(int argc, char** argv) {
  postgres_adapter db("dwh.camato.eu", "read", "dwh_production");

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
