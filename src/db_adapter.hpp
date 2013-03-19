#ifndef DB_ADAPTER_HPP
#define DB_ADAPTER_HPP

#include <vector>
#include <string>
#include <stdexcept>

namespace dbshell{

class db_adapter {

public:

  /**
   * Execute a query on this connection.
   *
   * @param query  The query string.
   * @return  The result of the query. The first element of the returned pair
   *          contains the column names, the second element contains the rows.
   * @throws  If no connection to the database was established or the query
   *          failed to execute.
   */
  virtual std::pair<std::vector<std::string>, std::vector<std::vector<std::string>>> query(std::string query) throw (std::runtime_error) = 0;

  /**
   * Cancel a running query asap. If no query is currently being executed
   * return immediately.
   */
  virtual void cancel() = 0;

};

} /* namespace dbshell */

#endif /* DB_ADAPTER_HPP */
