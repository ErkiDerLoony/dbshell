#include <iostream>

#include "postgres_adapter.hpp"
#include "readline.hpp"

int main(int argc, char** argv) {
  //postgres_adapter db("dwh.camato.eu", "read", "dwh_production");
  //db.query("SELECT * FROM keyword_dim LIMIT 1");

  dbshell::readline stdin("-> ");
  std::string line;

  do {
    line = stdin.read();
    std::cout << line << std::endl;
  } while (line != "quit");

  std::cout << "Bye." << std::endl;

  return 0;
}
