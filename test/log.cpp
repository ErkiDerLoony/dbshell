#include "log.hpp"

int main() {
  log::info << "Ein informativer Text." << std::endl;
  log::debug << "Eine Debug-Ausgabe." << log::endl;

  int i = 10;
  log::info << "Eine Ausgabe mit der Zahl " << i << "." << log::endl;
}
