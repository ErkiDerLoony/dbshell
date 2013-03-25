#ifndef DOUBLE_COLUMN_HPP
#define DOUBLE_COLUMN_HPP

#include "column.hpp"

#include <string>

namespace dbshell {

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

  std::string name() const;

  /**
   * Format a value according to this column’s layout.
   *
   * @param value  The value that shall be formatted.
   * @return  The given value formatted according to this column’s preferred
   *          layout (which may consider other values in this column).
   */
  std::string format(double value) const;

  alignment_type alignment() const;

private:

  const alignment_type _alignment;

};

} /* namespace dbshell */

#endif /* DOUBLE_COLUMN_HPP */
