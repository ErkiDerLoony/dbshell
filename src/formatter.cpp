/*
 * © Copyright 2013 by Edgar Kalkowski <eMail@edgar-kalkowski.de>
 * 
 * This file is part of the program dbshell.
 * 
 * The program dbshell is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "formatter.hpp"

using std::string;
using std::wstring;
using std::vector;
using std::stringstream;
using std::wstringstream;
using std::endl;

namespace dbshell {

inline wstring fill(long ms) {

  if (ms < 10) {
    wstringstream buffer;
    buffer << L"00" << ms;
    return buffer.str();
  }

  if (ms < 100) {
    wstringstream buffer;
    buffer << L"0" << ms;
    return buffer.str();
  }

  wstringstream buffer;
  buffer << ms;
  return buffer.str();
}

wstring format_duration(long ms) {
  long s = ms / 1000;
  ms %= 1000;

  if (s == 0) {
    wstringstream buffer;
    buffer << ms << " ms";
    return buffer.str();
  }

  long m = s / 60;
  s %= 60;

  if (m == 0) {
    wstringstream buffer;
    buffer << s << L"." << fill(ms) << L" s";
    return buffer.str();
  }

  long h = m / 60;
  m %= 60;

  if (h == 0) {
    wstringstream buffer;
    buffer << m << ":";
    if (s < 10) buffer << "0";
    buffer << s << " min";
    return buffer.str();
  }

  long d = h / 24;
  h %= 24;

  if (d == 0) {
    wstringstream buffer;
    buffer << h << ":";
    if (m < 10) buffer << "0";
    buffer << m << ":";
    if (s < 10) buffer << "0";
    buffer << s << " h";
    return buffer.str();
  }

  wstringstream buffer;
  buffer << d << "d " << h << ":";
  if (m < 10) buffer << "0";
  buffer << m << ":";
  if (s < 10) buffer << "0";
  buffer << s;
  return buffer.str();
}

} /* namespace dbshell */
