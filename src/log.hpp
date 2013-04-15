#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <sstream>

namespace dbshell {

namespace log {

enum level {
  DEBUG, INFO, WARNING
};

}

namespace detail {

class logger {

public:

  logger(log::level level);

  virtual ~logger();

  typedef std::basic_ostream<char, std::char_traits<char>> stdout;
  typedef stdout& (*stdendl)(detail::logger::stdout&);

  logger& operator<<(const std::wstring& t);

  logger& operator<<(const std::string& t);

  logger& operator<<(stdendl f);

private:

  log::level _level;

  std::wstringstream _buffer;

}; /* class logger */

} /* namespace detail */

namespace log {

extern std::wostream& handler;

extern detail::logger warning;
extern detail::logger info;
extern detail::logger debug;

}

} /* namespace dbshell */

#endif /* LOG_HPP */
