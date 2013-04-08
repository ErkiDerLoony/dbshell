#include "log.hpp"

using std::wostream;
using std::wcout;
using std::string;

using namespace dbshell;

wostream& detail::handler = wcout;

detail::logger::logger() {
}

detail::logger::~logger() {
}

detail::logger& detail::logger::operator<<(const string& text) {
  buffer << text;
  return *this;
}

detail::logger& detail::logger::operator<<(detail::logger::stdendl endl) {
  buffer << std::endl;
  return *this;
}
