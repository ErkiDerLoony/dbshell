#ifndef TABLE_HPP
#define TABLE_HPP

#include <string>
#include <vector>
#include <ostream>
#include <tuple>
#include <stdexcept>

namespace dbshell {

enum class alignment_type {
  LEFT, CENTER, RIGHT
};

/** A simple table that contains the results of a database query. */
class table {

public:

  /** Create a new empty table. */
  table();

  /** Destroy this table and free all allocated resources. */
  virtual ~table();

  /**
   * Access the number of columns in this table.
   *
   * @return  This table’s number of columns.
   */
  uint columns() const;

  /**
   * Access the number of rows in this table.
   *
   * @return This table’s number of rows.
   */
  uint rows() const;

  /**
   * Access a specific column of this table.
   *
   * @param index  The index of the column to access.
   * @throws  If the given index is out of range.
   */
  const std::wstring& column(const uint index) const throw(std::range_error);

  /**
   * Access the paragraph alignment of some column of this table.
   *
   * @param index  The index of the column whose paragraph alignment shall
   *               be retrieved.
   * @return  The paragraph alignment of the specified column (left, center,
   *          right).
   * @throws  If the given index in out of range.
   */
  const alignment_type& alignment(const uint index) const throw(std::range_error);

  /**
   * Access the alignment string of some column of this table.
   *
   * @param index  The index of the column whose alignment string shall be retrieved.
   * @return  The alignment string of the specified column.
   * @throws  If the given index is out of range.
   */
  const std::wstring& alignment_string(const uint index) const throw(std::range_error);

  /**
   * Add a column to this table.
   *
   * @param name  The name of the new column.
   * @param paragraph_alignment  Specify how values of the new column shall be
   *                             aligned in output (left, center, right).
   * @param alignment_character  Specify a character at whose first occurence
   *                             values of the new column shall be aligned in
   *                             output. The default value of '' (empty char)
   *                             means no special alignment.
   */
  void add(std::wstring name,
           alignment_type paragraph_alignment = alignment_type::LEFT,
           std::wstring alignment_character = L"");

  /**
   * @copydoc add(std::string, alignment_type, std::string)
   */
  void add_column(std::wstring name,
                  alignment_type paragraph_alignment = alignment_type::LEFT,
                  std::wstring alignment_character = L"");

  /**
   * Add a row of data to this table.
   *
   * @param row  The new row of data.
   * @throws  If the length of the given row of data does not match the number
   *          of columns of this table.
   */
  void add(std::vector<std::wstring> row) throw(std::runtime_error);

  /**
   * Add a row of data to this table.
   *
   * @param row  The new row of data.
   * @throws  If the length of the given row of data does not match the number
   *          of columns of this table.
   */
  void add(std::initializer_list<std::wstring> row) throw(std::runtime_error);

  /**
   * Access a row of data of this table.
   *
   * @param index  The index of the row to access.
   * @return  The row of data at the specified position.
   * @throws  If the specified index is out of range for this table.
   */
  const std::vector<std::wstring>& row(const uint index) const throw(std::range_error);

  /**
   * Access a cell of this table.
   *
   * @param row  The row index of the cell to access.
   * @param col  The column index of the cell to access.
   * @return  The value of this table at the specified position.
   * @throws  If the specified indices are invalid for this table.
   */
  std::wstring get(const uint row, const uint col) const throw(std::range_error);

  /**
   * Change a value of this table.
   *
   * @param row  The row index of the cell to update.
   * @param col  The column index of the cell to update.
   * @param value  The new value for the specified cell.
   * @throws  If one of the specified indices in out of range for this table.
   */
  void set(const uint row, const uint col, const std::wstring value) throw(std::range_error);

  /**
   * Access a row of data of this table.
   *
   * @param index  The index of the row to access.
   * @return  The row of data at the specified position.
   * @throws  If the specified index is out of range for this table.
   */
  const std::vector<std::wstring>& operator[](const uint index) const throw(std::range_error);

  const std::vector<std::vector<std::wstring>>::const_iterator begin() const;

  const std::vector<std::vector<std::wstring>>::const_iterator end() const;

private:

  std::vector<std::wstring> _columns;

  std::vector<alignment_type> _alignments;

  std::vector<std::wstring> _alignment_strings;

  std::vector<std::vector<std::wstring>> _rows;

};

} /* namespace dbshell */

std::wostream& operator<<(std::wostream& stream, const dbshell::table& table);

#endif /* TABLE_HPP */
