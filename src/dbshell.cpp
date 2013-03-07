#include "postgres_adapter.hpp"

int main(int argc, char** argv) {
  postgres_adapter db("dwh.camato.eu", "read");
  db.query("SELECT * FROM keyword_dim LIMIT 1");
  return 0;
}
