#ifndef COLUMN_HPP
#define COLUMN_HPP

#include <string>

namespace dbshell {

enum class alignment_type {

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
  std::string name() const;

  /**
   * Access the alignment of this column.
   *
   * @return  This column’s preferred way of how textual output shall be
   *          aligned.
   */
  alignment_type alignment() const;

private:

  const std::string _name;

};

} /* namespace dbshell */

#endif /* COLUMN_HPP */
