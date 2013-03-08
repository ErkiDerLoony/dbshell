#include <vector>
#include <string>

namespace dbshell {

std::vector<uint> size(const std::vector<std::string>& header, const std::vector<std::vector<std::string>>& rows);

std::string formatHeader(const std::vector<std::string>& header, const std::vector<uint>& sizes);

std::string formatRows(const std::vector<std::vector<std::string>>& rows, const std::vector<uint>& sizes);

std::string format(const std::vector<std::string>& header, const std::vector<std::vector<std::string>>& rows);

} /* namespace dbshell */
