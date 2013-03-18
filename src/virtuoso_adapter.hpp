#ifndef VIRTUOSO_ADAPTER_HPP
#define VIRTUOSO_ADAPTER_HPP

#include "db_adapter.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace dbshell {

class virtuoso_adapter : public db_adapter {

public:

  virtuoso_adapter(std::string host, std::string username);

  virtual ~virtuoso_adapter();

  virtual std::pair<std::vector<std::string>, std::vector<std::vector<std::string>>> query(std::string query) throw (std::runtime_error);

};

} /* namespace dbshell */

#endif /* VIRTUOSO_ADAPTER_HPP */
