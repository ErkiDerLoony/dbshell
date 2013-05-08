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
   * Dummy destructor to prevent memory leaks (nothing is allocated in this
   * purely abstract class and thus nothing has to be released).
   */
  virtual ~db_adapter() {}

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
