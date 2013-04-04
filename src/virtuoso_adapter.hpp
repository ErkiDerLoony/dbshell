#ifndef VIRTUOSO_ADAPTER_HPP
#define VIRTUOSO_ADAPTER_HPP

#include "db_adapter.hpp"

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

  SQLHANDLE environment;
  SQLHDBC connection;
  SQLHSTMT statement;

};

} /* namespace dbshell */

#endif /* VIRTUOSO_ADAPTER_HPP */
