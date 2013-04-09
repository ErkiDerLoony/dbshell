#include "table.hpp"

#include <iostream>

using namespace dbshell;
using std::wcout;
using std::endl;

int main(int argc, char** argv) {

  // Fix io of UTF-8 characters.
  std::ios_base::sync_with_stdio(false);
  std::wcout.imbue(std::locale("en_US.UTF_8"));

  table t;

  t.add(L"text", alignment_type::LEFT);
  t.add(L"text column", alignment_type::RIGHT);
  t.add(L"very „long“ value column", alignment_type::CENTER, L".");
  t.add(L"v", alignment_type::RIGHT, L".");

  t.add({ L"minuas a", L"minus a", L"-100.001", L"1400" });
  t.add({ L"asdf",     L"kkk",      L"0.25",  L"3" });
  t.add({ L"fasas",    L"fasas",    L"00000000000000",   L"20" });
  t.add({ L"plas as",  L"plas a",   L"0.001", L"1" });

  wcout << t << endl;
}
