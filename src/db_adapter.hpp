#ifndef DB_ADAPTER_HPP
#define DB_ADAPTER_HPP

#include "table.hpp"

#include <string>
#include <stdexcept>
#include <memory>

namespace dbshell{

class db_adapter {

public:

  /**
   * Execute a query on this connection.
   *
   * @param query  The query string.
   * @return  The result table of the query.
   * @throws  If no connection to the database was established or the query
   *          failed to execute.
   */
  virtual std::unique_ptr<table> query(std::string query) throw(std::runtime_error) = 0;

  /**
   * Cancel a running query asap. If no query is currently being executed
   * return immediately.
   */
  virtual void cancel() = 0;

};

} /* namespace dbshell */

#endif /* DB_ADAPTER_HPP */
