#include "query_failed.hpp"

query_failed::query_failed(std::string message) : _message(message) {
}

query_failed::~query_failed() noexcept(true) {
}

std::string query_failed::message() const {
  return _message;
}
