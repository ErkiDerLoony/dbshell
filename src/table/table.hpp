#ifndef TABLE_HPP
#define TABLE_HPP

#include <vector>
#include <string>
#include <memory>

namespace dbshell {

enum alignment_type {

  LEFT,

  CENTER,

  RIGHT

};

/** Abstract superclass for columns of a table. */
class column {

public:

  /**
   * Create a new column.
   *
   * @param name  The name of the new column.
   */
  explicit column(std::string name);

  /** Destroy this column. */
  virtual ~column();

  /**
   * Access the name of this column.
   *
   * @return  This column’s name.
   */
  std::string name();

  /**
   * Access the alignment of this column.
   *
   * @return  This column’s preferred way of how textual output shall be
   *          aligned.
   */
  alignment_type alignment();

private:

  std::string _name;

};

/** A column whose values are of type double. */
class double_column : public column {

public:

  /**
   * Create a new column that may contain values of type double.
   *
   * @param name  The name of the new column.
   */
  explicit double_column(std::string name);

  /** Destroy this column. */
  virtual ~double_column();

  std::string name();

  /**
   * Format a value according to this column’s layout.
   *
   * @param value  The value that shall be formatted.
   * @return  The given value formatted according to this column’s preferred layout (which may consider other values in this column).
   */
  std::string format(double value);

private:

};

class long_column : public column {

public:

  explicit long_column(std::string name);

  virtual ~long_column();

  std::string name();

  std::string format(long value);

  alignment_type alignment();

private:

};

/** A result structure for tables queried from a database. */
class table {

public:

  /** Create a new empty table without any columns. */
  table();

  /** Destroy a table and free it’s resources. */
  virtual ~table();

  /**
   * Add a new column to this table.
   *
   * @param column  A pointer to the new column.
   */
  void add_column(std::shared_ptr<column> column);

  static bool debug;

private:

  std::vector<std::shared_ptr<column>> _columns;

};

} /* namespace dbshell */

#endif /* TABLE_HPP */
