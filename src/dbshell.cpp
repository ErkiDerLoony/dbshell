#include "postgres_adapter.hpp"
#include "readline_adapter.hpp"

#include <iostream>

int main(int argc, char** argv) {
  //postgres_adapter db("dwh.camato.eu", "read", "dwh_production");
  //db.query("SELECT * FROM keyword_dim LIMIT 1");

  dbshell::prompt = "->";
  std::string line;

  do {
    line = dbshell::readline();
    std::cout << line << std::endl;
  } while (line != "quit");

  std::cout << "Bye." << std::endl;

  return 0;
}
