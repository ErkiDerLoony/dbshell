#include "getopt.hpp"

#include <unistd.h>
#include <iostream>

using namespace dbshell;
using std::pair;
using std::string;
using std::map;
using std::list;
using std::cout;
using std::endl;

getopt::getopt(int argc, char** argv) {

  for (int i = 1; i < argc; i++) {
    string arg(argv[i]);

    if (arg == "--") {
      continue;
    }

    if (arg.substr(0, 2) == "--") {
      // Parse long options.
      arg = arg.substr(2);
      size_t found = arg.find("=");

      if (found != string::npos) {
        string key = arg.substr(0, found);
        string val = arg.substr(found + 1);
        _options.insert(pair<string, string>(key, val));
      } else {

        if (i + 1 == argc || string(argv[i + 1]).substr(0, 1) == "-" ||
            string(argv[i + 1]) == "--") {
          _options.insert(pair<string, string>(arg, ""));
        } else {
          _options.insert(pair<string, string>(arg, argv[i + 1]));
          i++;
        }
      }

    } else if (arg.substr(0, 1) == "-") {
      // Parse short options.
      arg = arg.substr(1);

      if (arg.size() > 1) {
        string key = arg.substr(0, 1);
        string val = arg.substr(1);
        _options.insert(pair<string, string>(key, val));
      } else {

        if (i + 1 == argc || string(argv[i + 1]).substr(0, 1) == "-" ||
            string(argv[i + 1]) == "--") {
          _options.insert(pair<string, string>(arg, ""));
        } else {
          _options.insert(pair<string, string>(arg, argv[i + 1]));
          i++;
        }
      }
    } else {
      // Parse final list of strings (e.g. filenames).

      for (int j = i; j < argc; j++) {
        _list.push_back(argv[j]);
      }

      break;
    }
  }
}

getopt::~getopt() {
}

list<string> getopt::list() const {
  return _list;
}

map<string, string>::iterator getopt::begin() {
  return _options.begin();
}

map<string, string>::iterator getopt::end() {
  return _options.end();
}

string getopt::get(string key) const {
  return _options.at(key);
}
