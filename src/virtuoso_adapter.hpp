#ifndef VIRTUOSO_ADAPTER_HPP
#define VIRTUOSO_ADAPTER_HPP

#include "db_adapter.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace dbshell {

/**
 * This class represents a connection to a virtuoso database.
 *
 * @author Edgar Kalkowski <eMail@edgar-kalkowski.de>
 */
class virtuoso_adapter : public db_adapter {

public:

  /**
   * Create a new adapter.
   *
   * @param host  The hostname of the database server to connect to.
   * @param username  The username to use for login.
   */
  virtuoso_adapter(std::string host, std::string username);

  virtual ~virtuoso_adapter();

  virtual std::pair<std::vector<std::string>, std::vector<std::vector<std::string>>> query(std::string query) throw (std::runtime_error);

  virtual void cancel();

};

} /* namespace dbshell */

#endif /* VIRTUOSO_ADAPTER_HPP */
