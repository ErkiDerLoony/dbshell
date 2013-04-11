#include "table.hpp"

#include <iostream>

using namespace dbshell;
using std::wcout;
using std::endl;

int main(int argc, char** argv) {

  // Fix io of UTF-8 characters.
  std::ios_base::sync_with_stdio(false);
  std::wcout.imbue(std::locale(""));

  table t0;

  t0.add(L"l", alignment_type::LEFT);
  t0.add(L"c", alignment_type::CENTER);
  t0.add(L"r", alignment_type::RIGHT);
  t0.add(L"very long l", alignment_type::LEFT);
  t0.add(L"very long c", alignment_type::CENTER);
  t0.add(L"very long r", alignment_type::RIGHT);

  t0.add({L"text", L"text", L"text", L"text", L"text", L"text"});
  t0.add({L"gg", L"gg", L"gg", L"gg", L"gg", L"gg"});
  t0.add({L"a", L"b", L"c", L"d", L"e", L"f"});

  wcout << t0 << endl;

  table t1;

  t1.add(L"l", alignment_type::LEFT, L".");
  t1.add(L"c", alignment_type::CENTER, L".");
  t1.add(L"r", alignment_type::RIGHT, L".");
  t1.add(L"very long l", alignment_type::LEFT, L".");
  t1.add(L"very long c", alignment_type::CENTER, L".");
  t1.add(L"very long r", alignment_type::RIGHT, L".");

  t1.add({L"0", L"0", L"0", L"0", L"0", L"0"});
  t1.add({L"100", L"100", L"100", L"100", L"100", L"100"});
  t1.add({L"0.234", L"0.234", L"0.234", L"0.234", L"0.234", L"0.234"});
  t1.add({L"-0.234", L"-0.234", L"-0.234", L"-0.234", L"-0.234", L"-0.234"});

  wcout << t1 << endl;
}
