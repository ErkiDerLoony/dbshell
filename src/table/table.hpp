#ifndef TABLE_HPP
#define TABLE_HPP

#include <vector>
#include <string>
#include <memory>

namespace dbshell {

/** Abstract superclass for columns of a table. */
template<typename T>
class column {

public:

  typedef T type;

  column(T value);

  virtual ~column();

  void print();

private:

  T _value;

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
  //  void add_column(std::shared_ptr<column> column);

  static bool debug;

private:

  //  std::vector<std::shared_ptr<column>> _columns;

};

} /* namespace dbshell */

#endif /* TABLE_HPP */
