#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <sstream>

namespace dbshell {

namespace detail {

extern std::wostream& handler;

class logger {

public:

  logger();

  virtual ~logger();

  typedef std::basic_ostream<char, std::char_traits<char>> stdout;
  typedef stdout& (*stdendl)(detail::logger::stdout&);

  logger& operator<<(const std::string& t);

  logger& operator<<(stdendl f);

private:

  std::stringstream buffer;

};

}

namespace log {

detail::logger info;
detail::logger debug;

}

}

#endif /* LOG_HPP */
