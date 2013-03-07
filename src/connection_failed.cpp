#include "connection_failed.hpp"

connection_failed::connection_failed(std::string message)
  : _message(message) {
}

connection_failed::~connection_failed() noexcept(true) {
}

std::string connection_failed::message() {
  return _message;
}
