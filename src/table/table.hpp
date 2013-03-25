#ifndef TABLE_HPP
#define TABLE_HPP

#include "column.hpp"
#include "row.hpp"

#include <vector>
#include <string>
#include <memory>
#include <iostream>

namespace dbshell {

/** A result structure for tables queried from a database. */
class table final {

public:

  /** Create a new empty table without any columns. */
  table();

  /** Destroy a table and free it’s resources. */
  virtual ~table();

  /**
   * Access all columns of this table.
   *
   * @return  A reference to this table’s columns.
   */
  const std::vector<std::shared_ptr<column>>& columns() const;

  /**
   * Add a new column to this table.
   *
   * @param column  A pointer to the new column.
   */
  void add_column(std::shared_ptr<column> column);

  void add_row(std::shared_ptr<row> row);

  static bool debug;

private:

  std::vector<std::shared_ptr<column>> _columns;

  std::vector<std::shared_ptr<row>> _rows;

};

} /* namespace dbshell */

/**
 * Output tables.
 *
 * @param stream  The output stream to which a textual representation of a table
 *                shall be appended.
 * @param table  The table whose textual representation shall be appended to the
 *               given output stream.
 * @return  A reference to the given stream with the given table appended to it.
 */
std::ostream& operator<<(std::ostream& stream, const dbshell::table& table);

#endif /* TABLE_HPP */
