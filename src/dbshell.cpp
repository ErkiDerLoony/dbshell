#include "postgres_adapter.hpp"
#include "readline_adapter.hpp"
#include "formatter.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

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
      pair<vector<string>, vector<vector<string>>> table = db.query(line);
      cout << format(table.first, table.second) << endl;
    } catch (runtime_error e) {
      cout << e.what();
    }
  }

  cout << "Bye." << endl;

  return 0;
}
