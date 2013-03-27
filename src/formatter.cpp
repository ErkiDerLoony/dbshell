#include "formatter.hpp"

using std::string;
using std::wstring;
using std::vector;
using std::stringstream;
using std::wstringstream;
using std::endl;

namespace dbshell {

vector<uint> size(const vector<string>& header, const vector<vector<string>>& rows) {
  vector<uint> sizes;

  for (uint i = 0; i < header.size(); i++) {
    sizes.push_back(header[i].size());
  }

  for (vector<string> row : rows) {

    for (uint i = 0; i < header.size(); i++) {
      const uint size = row[i].size();

      if (size > sizes[i]) {
        sizes[i] = size;
      }
    }
  }

  return sizes;
}

inline wstring fill(long ms) {

  if (ms < 10) {
    wstringstream buffer;
    buffer << L"00" << ms;
    return buffer.str();
  }

  if (ms < 100) {
    wstringstream buffer;
    buffer << L"0" << ms;
    return buffer.str();
  }

  wstringstream buffer;
  buffer << ms;
  return buffer.str();
}

wstring format_duration(long ms) {
  long s = ms / 1000;
  ms %= 1000;

  if (s == 0) {
    wstringstream buffer;
    buffer << ms << " ms";
    return buffer.str();
  }

  long m = s / 60;
  s %= 60;

  if (m == 0) {
    wstringstream buffer;
    buffer << s << L"." << fill(ms) << L" s";
    return buffer.str();
  }

  long h = m / 60;
  m %= 60;

  if (h == 0) {
    wstringstream buffer;
    buffer << m << ":";
    if (s < 10) buffer << "0";
    buffer << s << " min";
    return buffer.str();
  }

  long d = h / 24;
  h %= 24;

  if (d == 0) {
    wstringstream buffer;
    buffer << h << ":";
    if (m < 10) buffer << "0";
    buffer << m << ":";
    if (s < 10) buffer << "0";
    buffer << s << " h";
    return buffer.str();
  }

  wstringstream buffer;
  buffer << d << "d " << h << ":";
  if (m < 10) buffer << "0";
  buffer << m << ":";
  if (s < 10) buffer << "0";
  buffer << s;
  return buffer.str();
}

string format_header(const vector<string>& header, const vector<uint>& sizes, stringstream& buffer) {

  for (uint i = 0; i < header.size(); i++) {
    buffer << "│ ";

    for (uint k = 0; k < (sizes[i] - header[i].length()) / 2; k++) {
      buffer << " ";
    }

    buffer << header[i] << " ";

    for (uint k = 0; k < (sizes[i] - header[i].length() + 1) / 2; k++) {
      buffer << " ";
    }
  }

  buffer << "│" << endl;
  return buffer.str();
}

string format_rows(const vector<vector<string>>& rows, const vector<uint>& sizes, stringstream& buffer) {

  for (vector<string> row : rows) {

    for (uint i = 0; i < row.size(); i++) {
      buffer << "│";

      for (uint k = 0; k < 1 + (sizes[i] - row[i].length() + 1) / 2; k++) {
        buffer << " ";
      }

      buffer << row[i];

      for (uint k = 0; k < 1 + (sizes[i] - row[i].length()) / 2; k++) {
        buffer << " ";
      }
    }

    buffer << "│" << endl;
  }

  return buffer.str();
}

string format(const vector<string>& header, const vector<vector<string>>& rows) {
  vector<uint> sizes = size(header, rows);
  stringstream buffer;
  buffer << "┌";

  for (uint i = 0; i < sizes.size(); i++) {

    for (uint k = 0; k < sizes[i] + 2; k++) {
      buffer << "─";
    }

    if (i < sizes.size() - 1) {
      buffer << "┬";
    } else {
      buffer << "┐" << endl;
    }
  }

  format_header(header, sizes, buffer);

  buffer << "├";

  for (uint i = 0; i < sizes.size(); i++) {

    for (uint k = 0; k < sizes[i] + 2; k++) {
      buffer << "─";
    }

    if (i < sizes.size() - 1) {
      buffer << "┼";
    } else {
      buffer << "┤" << endl;
    }
  }

  format_rows(rows, sizes, buffer);

  buffer << "└";

  for (uint i = 0; i < sizes.size(); i++) {

    for (uint k = 0; k < sizes[i] + 2; k++) {
      buffer << "─";
    }

    if (i < sizes.size() - 1) {
      buffer << "┴";
    } else {
      buffer << "┘";
    }
  }

  return buffer.str();
}

} /* namespace dbshell */
