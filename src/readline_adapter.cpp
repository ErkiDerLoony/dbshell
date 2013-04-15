#include "readline_adapter.hpp"

#include <errno.h>
#include <pwd.h>
#include <unistd.h>
#include <iostream>

extern "C" {
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
}

using namespace dbshell;
using std::string;

string dbshell::prompt = "->";

string dbshell::readline() {
  string line;

  do {
    char* buffer;

    if (line.length() > 0) {
      buffer = ::readline("");
    } else {
      buffer = ::readline(dbshell::prompt.c_str());
    }

    if (buffer != NULL) {

      if (line.length() > 0 && line != ";") {
        line += "\n";
      }

      line += buffer;
      free(buffer);
    }

  } while (line.length() < 1 || (line.substr(line.length() - 1, line.length()) != ";" && line.substr(0, 1) != "\\"));

  ::add_history(line.c_str());
  return line;
}

void dbshell::clear_input() {
  rl_replace_line("", 0);
}

class history_management {

public:

  history_management() {
    filename = string(getpwuid(getuid())->pw_dir);
    filename += "/.config/dbshell_history";

    int result = ::read_history(filename.c_str());

    if (result != 0) {
      perror("load readline history");
    }
  }

  virtual ~history_management() {
    int result = ::write_history(filename.c_str());

    if (result != 0) {
      perror("save readline history");
    }
  }

private:

  string filename;

};

const history_management _r;
