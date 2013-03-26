#include "readline_adapter.hpp"
#include "formatter.hpp"
#include "db_adapter.hpp"
#include "virtuoso_adapter.hpp"
#include "postgres_adapter.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <memory>

#include <signal.h>
#include <sys/time.h>

using namespace dbshell;
using std::unique_ptr;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::pair;
using std::stringstream;
using std::runtime_error;

namespace dbshell {
  bool running = false;
  unique_ptr<db_adapter> connection = unique_ptr<db_adapter>(nullptr);
}

void handler(int sig) {

  if (!running) {
    return;
  }

  dbshell::connection->cancel();
}

int main(int argc, char** argv) {

  if (argc < 2) {
    cerr << "You must specify a connection string on the command line!" << endl;
    return 1;
  }

  string arg(argv[1]);
  auto index = arg.find("://");

  if (index == string::npos) {
    cerr << "Could not find schema in connection string »" << arg << "«!" << endl;
    return 1;
  }

  string schema = arg.substr(0, index);
  string rest = arg.substr(index + 3, arg.length());

  if (rest == "") {
    cerr << "No hostname in connection string »" << arg << "«!" << endl;
    return 1;
  }

  string username = "";
  index = rest.find("@");

  if (index != string::npos) {
    username = rest.substr(0, index);
    rest = rest.substr(index + 1, rest.length());
  }

  string host = "";
  index = rest.find("/");

  if (index != string::npos) {
    host = rest.substr(0, index);
    rest = rest.substr(index + 1, rest.length());
  }

  string database = rest;

  if (schema == "psql" || schema == "postgres" || schema == "postgresql") {
    dbshell::connection = unique_ptr<db_adapter>(new postgres_adapter(host, username, database));
  } else if (schema == "virtuoso") {
    dbshell::connection = unique_ptr<db_adapter>(new virtuoso_adapter(host, username));
  } else {
    cerr << "Unsupported database type: " << schema << endl;
    return 1;
  }

  signal(SIGABRT, &handler);
  signal(SIGTERM, &handler);
  signal(SIGINT, &handler);

  prompt = "-> ";
  string line;

  while (true) {
    line = readline();

    if (line.substr(0, 1) != "\\") {
      line = line.substr(0, line.length() - 1);
    } else {

      if (line.substr(line.length() - 1, line.length()) == ";") {
        line = line.substr(0, line.length() - 1);
      }
    }

    if (line == "quit" || line == "\\q") {
      break;
    }

    try {
      struct timeval start;
      gettimeofday(&start, nullptr);
      dbshell::running = true;
      unique_ptr<table> table = dbshell::connection->query(line);
      dbshell::running = false;
      struct timeval end;
      gettimeofday(&end, nullptr);
      const long diff = end.tv_sec * 1000 + end.tv_usec / 1000 - start.tv_sec * 1000 - start.tv_usec / 1000;
      cout << *table << endl;
      cout << table->rows() << " row";
      if (table->rows() != 1) cout << "s";
      cout << ", " << format_duration(diff) << endl;
    } catch (runtime_error e) {
      cout << e.what();
    }
  }

  cout << "Bye." << endl;

  return 0;
}
