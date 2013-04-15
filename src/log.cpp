#include "log.hpp"

using std::char_traits;
using std::basic_ostream;
using std::wostream;
using std::wcout;
using std::wstring;
using std::string;

using namespace log;

wostream& log::handler = wcout;

detail::logger::logger(loglevel level) : _level(level) {
}

detail::logger::~logger() {
}

detail::logger& detail::logger::operator<<(const wstring& text) {
  _buffer << text;
  return *this;
}

detail::logger& detail::logger::operator<<(const string& text) {

  for (char c : text) {
    _buffer << c;
  }

  return *this;
}

detail::logger& detail::logger::operator<<(const int& i) {
  _buffer << i;
  return *this;
}

detail::logger& detail::logger::operator<<(stdout& (*f)(stdout&)) {
  handler << _level << ": " << _buffer.str() << std::endl;
  _buffer.str(L"");
  return *this;
}

namespace log {

basic_ostream<char, char_traits<char>>& (*endl)(basic_ostream<char, char_traits<char>>&);
bool use_colour = true;
loglevel level = INFO;

detail::logger warning(log::WARNING);
detail::logger info(log::INFO);
detail::logger debug(log::DEBUG);

} /* namespace log */
