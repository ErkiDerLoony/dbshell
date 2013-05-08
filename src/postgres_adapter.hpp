/*
 * © Copyright 2013 by Edgar Kalkowski <eMail@edgar-kalkowski.de>
 * 
 * This file is part of the program dbshell.
 * 
 * The program dbshell is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef POSTGRES_ADAPTER_HPP
#define POSTGRES_ADAPTER_HPP

extern "C" {
#include <libpq-fe.h>
}

#include "db_adapter.hpp"

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

  virtual std::unique_ptr<table> query(std::string query) throw (std::runtime_error) override;

  virtual void cancel() override;

private:

  PGconn* _connection;

  std::string _host;

  std::string _username;

  std::string _database;

}; /* class postgres_adapter */

} /* namespace dbshell */

#endif /* POSTGRES_ADAPTER_HPP */
