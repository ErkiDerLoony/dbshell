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
   * Format a value according to the other values in this column.
   *
   * @param value  The value to format.
   * @return  A string representation of the given value.
   */
  virtual std::string format(long value) const;

  virtual std::string str() const override;

private:

};

} /* namespace dbshell */

#endif /* LONG_COLUMN_HPP */
