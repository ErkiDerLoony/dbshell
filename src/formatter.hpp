#include <vector>
#include <string>
#include <sstream>

namespace dbshell {

std::vector<uint> size(const std::vector<std::string>& header, const std::vector<std::vector<std::string>>& rows);

std::string format_header(const std::vector<std::string>& header, const std::vector<uint>& sizes, std::stringstream& buffer);

std::string format_rows(const std::vector<std::vector<std::string>>& rows, const std::vector<uint>& sizes, std::stringstream& buffer);

std::string format(const std::vector<std::string>& header, const std::vector<std::vector<std::string>>& rows);

std::string format_duration(const long ms);

} /* namespace dbshell */
