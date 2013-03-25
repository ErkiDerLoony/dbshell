#ifndef LONG_COLUMN_HPP
#define LONG_COLUMN_HPP

#include "column.hpp"

namespace dbshell {

class column;

/** A column whose values are of type long. */
class long_column : public column {

public:

  /**
   * Create a new column.
   *
   * @param name  The name of this new column.
   */
  explicit long_column(std::string name);

  /** Destroy this column and release acquired resources. */
  virtual ~long_column();

  /** 
   * Access the name of this column.
   *
   * @return  This column’s name.
   */
  std::string name() const;

  /**
   * Format a value according to the other values in this column.
   *
   * @param value  The value to format.
   * @return  A string representation of the given value.
   */
  std::string format(long value) const;

  /**
   * Access the alignment type of this column that specifies how values of this
   * column should be arranged with respect to other values in output.
   * 
   * @return  Left, center or right alignment.
   */
  alignment_type alignment() const;

private:

  const alignment_type _alignment;

};

} /* namespace dbshell */

#endif /* LONG_COLUMN_HPP */
