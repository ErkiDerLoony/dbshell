#include "log.hpp"

using std::wostream;
using std::wcout;
using std::wstring;
using std::string;

using namespace dbshell;

wostream& log::handler = wcout;

detail::logger::logger(log::level level) : _level(level) {
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

detail::logger& detail::logger::operator<<(detail::logger::stdendl endl) {
  log::handler << _level << ": " << _buffer.str() << std::endl;
  _buffer.str(L"");
  return *this;
}

namespace dbshell {

namespace log {

detail::logger warning(log::WARNING);
detail::logger info(log::INFO);
detail::logger debug(log::DEBUG);

} /* namespace log */

} /* namespace dbshell */
