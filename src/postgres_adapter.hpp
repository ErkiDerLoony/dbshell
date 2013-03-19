#ifndef POSTGRES_ADAPTER_HPP
#define POSTGRES_ADAPTER_HPP

extern "C" {
#include <libpq-fe.h>
}

#include "db_adapter.hpp"

#include <string>
#include <vector>
#include <stdexcept>

namespace dbshell {

/**
 * This class wraps a connection to some PostgreSQL database.
 *
 * @author Edgar Kalkowski <eMail@edgar-kalkowski.de>
 */
class postgres_adapter : public db_adapter {

public:

  /**
   * Create a new adapter that connects to some PostgreSQL database.
   *
   * @param host  The hostname of the database server.
   * @param username  The username to use for login.
   * @param database  The name of the database to query.
   * @throws  If no connection to the database could be established.
   */
  postgres_adapter(std::string host, std::string username, std::string database) throw(std::runtime_error);

  /** Disconnect from the database and release all resources. */
  virtual ~postgres_adapter();

  virtual std::pair<std::vector<std::string>, std::vector<std::vector<std::string>>> query(std::string query) throw (std::runtime_error);

  virtual void cancel();

private:

  PGconn* _connection;

  std::string _host;

  std::string _username;

  std::string _database;

}; /* class postgres_adapter */

} /* namespace dbshell */

#endif /* POSTGRES_ADAPTER_HPP */
