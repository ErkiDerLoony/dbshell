#ifndef QUERY_FAILED_HPP
#define QUERY_FAILED_HPP

#include <stdexcept>

class query_failed : public std::exception {

public:

  query_failed(std::string message);

  virtual ~query_failed() noexcept(true);

  std::string message() const;

private:

  std::string _message;

};

#endif /* QUERY_FAILED_HPP */
