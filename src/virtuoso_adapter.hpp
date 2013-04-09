#ifndef VIRTUOSO_ADAPTER_HPP
#define VIRTUOSO_ADAPTER_HPP

#include "db_adapter.hpp"
#include "prefixes.hpp"

#include <sql.h>

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
   * @throws  If no connection to the specified database could be established.
   */
  virtuoso_adapter(std::string host, std::string username) throw(std::runtime_error);

  virtual ~virtuoso_adapter();

  virtual std::unique_ptr<table> query(std::string query) throw (std::runtime_error) override;

  virtual void cancel() override;

private:

  SQLHANDLE environment = SQL_NULL_HANDLE;
  SQLHANDLE connection = SQL_NULL_HANDLE;
  SQLHANDLE statement = SQL_NULL_HANDLE;

  prefixes _prefixes;

  /**
   * Try to read the password for a user from ~/.vtpass (similar to PostgreSQL’s
   * ~/.pgpass). The password file should contain one entry of the form
   * “<username> <password>” per line (separation character is a single space).
   *
   * @param username  The username whose password shall be retrieved.
   * @return  The password for the given user or a {@code nullptr} if the
   *          password file does not contain a password for the specified
   *          username.
   */
  std::unique_ptr<std::string> pwd(const std::string& username);

};

} /* namespace dbshell */

#endif /* VIRTUOSO_ADAPTER_HPP */
