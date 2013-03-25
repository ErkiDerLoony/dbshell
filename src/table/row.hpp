#ifndef ROW_HPP
#define ROW_HPP

#include "column.hpp"

#include <memory>
#include <map>
#include <ostream>

namespace dbshell {

class row {

public:

  row();

  virtual ~row();

  void set(std::shared_ptr<column> column, std::shared_ptr<void> value);

private:

  std::map<std::shared_ptr<column>, std::shared_ptr<void>> _values;

};

} /* namespace dbshell */

std::ostream& operator<<(std::ostream& stream, const dbshell::row& row);

#endif /* ROW_HPP */
