/*
 * © Copyright 2013 by Edgar Kalkowski <eMail@edgar-kalkowski.de>
 * 
 * This file is part of the program dbshell.
 * 
 * The program dbshell is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
