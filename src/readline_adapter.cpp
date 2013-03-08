#include "readline_adapter.hpp"

extern "C" {
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
}

namespace dbshell {

std::string prompt = "->";

std::string readline() {
  char* buffer = ::readline(prompt.c_str());

  if (buffer != NULL) {
    std::string line = buffer;
    add_history(line.c_str());
    free(buffer);
    return line;
  } else {
    return std::string();
  }
}

} /* namespace dbshell */
