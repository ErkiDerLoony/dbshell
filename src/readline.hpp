#ifndef READLINE_HPP
#define READLINE_HPP

extern "C" {
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
}

#include <string>

namespace dbshell {

class readline {

public:
  readline(const std::string& prompt);
  virtual ~readline();
  std::string read();

private:
  std::string _prompt;

}; /* class readline */

} /* namespace dbshell */

#endif /* READLINE_HPP */
