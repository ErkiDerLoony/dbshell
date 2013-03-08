#include "postgres_adapter.hpp"
#include "readline_adapter.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace dbshell;
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::pair;
using std::stringstream;

namespace dbshell {

vector<uint> size(vector<string> header, vector<vector<string>> rows) {
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

string formatHeader(const vector<string>& header, const vector<uint>& sizes) {
  stringstream buffer;

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

string formatRows(const vector<vector<string>>& rows, const vector<uint>& sizes) {
  stringstream buffer;

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

  buffer << formatHeader(header, sizes);

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

  buffer << formatRows(rows, sizes);

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

int main(int argc, char** argv) {
  postgres_adapter db("dwh.camato.eu", "read", "dwh_production");

  prompt = "-> ";
  string line;

  while (true) {
    line = readline();

    if (line == "quit") {
      break;
    }

    pair<vector<string>, vector<vector<string>>> table = db.query(line);
    cout << format(table.first, table.second) << endl;
  }

  cout << "Bye." << endl;

  return 0;
}
