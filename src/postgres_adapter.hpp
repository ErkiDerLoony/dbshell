#ifndef POSTGRES_ADAPTER_HPP
#define POSTGRES_ADAPTER_HPP

extern "C" {

#include <libpq-fe.h>

}

#include <string>

class postgres_adapter {

public:

  postgres_adapter(std::string host, std::string username);

  virtual ~postgres_adapter();

  virtual std::string query(std::string query);

private:

  PGconn* _connection;

  std::string _host;

  std::string _username;

};

#endif /* POSTGRES_ADAPTER_HPP */
