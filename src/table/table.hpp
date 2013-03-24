#ifndef TABLE_HPP
#define TABLE_HPP

#include <vector>
#include <string>
#include <memory>

namespace dbshell {

/** Abstract superclass for columns of a table. */
class column {

public:

  explicit column(std::string name);

  virtual ~column();

  std::string name();

private:

  std::string _name;

};

class double_column : public column {

public:

  explicit double_column(std::string name);

  virtual ~double_column();

  std::string name();

  std::string format(double value);

private:

};

class long_column : public column {

public:

  explicit long_column(std::string name);

  virtual ~long_column();

  std::string name();

  std::string format(long value);

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
