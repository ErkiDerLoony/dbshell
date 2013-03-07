#ifndef CONNECTION_FAILED_HPP
#define CONNECTION_FAILED_HPP

#include <stdexcept>

class connection_failed : public std::exception {

public:

  explicit connection_failed(std::string message);

  virtual ~connection_failed() noexcept(true);

  std::string message();

private:

  std::string _message;

};

#endif /* CONNECTION_FAILED_HPP */
