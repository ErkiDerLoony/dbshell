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
