#include "readline_adapter.hpp"

#include <iostream>

extern "C" {
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
}

namespace dbshell {

std::string prompt = "->";

std::string readline() {
  std::string line;

  do {
    char* buffer;

    if (line.length() > 0) {
      buffer = ::readline("");
    } else {
      buffer = ::readline(prompt.c_str());
    }

    if (buffer != NULL) {

      if (line.length() > 0) {
        line += "\n";
      }

      line += buffer;
      free(buffer);
    }

  } while (line.length() < 1 || line.substr(line.length() - 1, line.length()) != ";");

  ::add_history(line.c_str());
  return line;
}

} /* namespace dbshell */
