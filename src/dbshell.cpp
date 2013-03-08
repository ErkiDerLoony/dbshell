#include "postgres_adapter.hpp"
#include "readline_adapter.hpp"

#include <iostream>

int main(int argc, char** argv) {
  postgres_adapter db("dwh.camato.eu", "read", "dwh_production");

  dbshell::prompt = "-> ";
  std::string line;

  while (true) {
    line = dbshell::readline();

    if (line == "quit") {
      break;
    }

    db.query(line);
  }

  std::cout << "Bye." << std::endl;

  return 0;
}
