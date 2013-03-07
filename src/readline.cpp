#include "readline.hpp"

namespace dbshell {

readline::readline(const std::string& prompt) : _prompt(prompt) {
}

readline::~readline() {
}

std::string readline::read() {
  char* buffer = ::readline(_prompt.c_str());

  if (buffer != NULL) {
    std::string line = buffer;
    free(buffer);
    return line;
  } else {
    return std::string();
  }
}

} /* namespace dbshell */
