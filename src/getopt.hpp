#ifndef GETOPT_HPP
#define GETOPT_HPP

#include <string>
#include <list>
#include <map>

namespace dbshell {

class getopt {

public:

  getopt(int argc, char** argv);

  virtual ~getopt();

  bool contains(std::string key) const;

  std::string get(std::string key) const;

  std::list<std::string> list() const;

  std::map<std::string, std::string>::iterator begin();

  std::map<std::string, std::string>::iterator end();

private:

  std::map<std::string, std::string> _options;

  std::list<std::string> _list;

};

}

#endif /* GETOPT_HPP */
