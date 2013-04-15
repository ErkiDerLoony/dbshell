#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <sstream>

namespace log {

enum loglevel {
  DEBUG, INFO, WARNING
};

typedef std::basic_ostream<char, std::char_traits<char>> stdout;

namespace detail {

class logger final {

public:

  logger(loglevel level);
  virtual ~logger();
  logger& operator<<(const std::wstring& t);
  logger& operator<<(const std::string& t);
  logger& operator<<(const int& i);
  logger& operator<<(stdout& (*f)(stdout&));

private:

  loglevel _level;
  std::wstringstream _buffer;

}; /* class logger */

} /* namespace detail */

extern std::wostream& handler;
extern loglevel level;
extern bool use_colour;
extern stdout& (*endl)(stdout&);

extern detail::logger warning;
extern detail::logger info;
extern detail::logger debug;

} /* namespace log */

#endif /* LOG_HPP */
