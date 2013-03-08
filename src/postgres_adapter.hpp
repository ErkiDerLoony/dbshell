#ifndef POSTGRES_ADAPTER_HPP
#define POSTGRES_ADAPTER_HPP

extern "C" {
#include <libpq-fe.h>
}

#include <string>
#include <stdexcept>

/**
 * This class wraps a connection to some PostgreSQL database.
 *
 * @author Edgar Kalkowski <eMail@edgar-kalkowski.de>
 */
class postgres_adapter {

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

  /**
   * Execute a query on this connection.
   *
   * @param query  The query string.
   * @return  The result of the query.
   * @throws  If no connection to the database was established.
   */
  virtual std::string query(std::string query) throw (std::runtime_error);

private:

  PGconn* _connection;

  std::string _host;

  std::string _username;

  std::string _database;

};

#endif /* POSTGRES_ADAPTER_HPP */
