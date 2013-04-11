#include "table.hpp"
#include "prefixes.hpp"

#include <iostream>

using namespace dbshell;
using std::wcout;
using std::endl;

int main() {
  std::ios_base::sync_with_stdio(false);
  wcout.imbue(std::locale(""));

  table t;

  t.add(L"Subject");
  t.add(L"Predicate");
  t.add(L"Object");

  t.add({L"<http://dbpedia.org/resource/Olpe>",
        L"<http://dbpedia.org/property/bürgermeister>",
        L"\"Horst Müller\"@en"});

  prefixes p;
  p.prefix(t);

  wcout << t << endl;
}
