#ifndef COLUMN_HPP
#define COLUMN_HPP

#include <string>
#include <memory>
#include <iostream>

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
   * @param alignment  Specifies how values of this column shall be aligned in
   *                   output.
   */
  explicit column(std::string name, alignment_type alignment);

  /** Destroy this column. */
  virtual ~column();

  /**
   * Access the name of this column.
   *
   * @return  This column’s name.
   */
  virtual std::string name() const final;

  /**
   * Access the alignment of this column.
   *
   * @return  This column’s preferred way of how textual output shall be
   *          aligned.
   */
  virtual alignment_type alignment() const final;

  /**
   * Create a textual representation of this column.
   *
   * @return  A textual representation of this column.
   */
  virtual std::string str() const = 0;

private:

  const std::string _name;

  const alignment_type _alignment;

};

} /* namespace dbshell */

/**
 * Output columns.
 *
 * @param stream  The stream to which a textual representation of the given
 *                column shall be appended.
 * @param column  The column whose textual representation shall be appended
 *                to the given stream.
 * @return  A reference to the stream with the appended column.
 */
std::ostream& operator<<(std::ostream& stream, const dbshell::column& column);

#endif /* COLUMN_HPP */
