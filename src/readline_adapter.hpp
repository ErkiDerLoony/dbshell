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

#ifndef READLINE_HPP
#define READLINE_HPP

#include <string>

/**
 * Main namespace of the dbshell program.
 *
 * @author Edgar Kalkowski <eMail@edgar-kalkowski.de>
 */
namespace dbshell {

/** The prompt to use when requesting user input. */
extern std::string prompt;

/**
 * Wrap the GNU readline library to get a line of text from the terminal.
 * The line is actually constructed from multiple lines until one line
 * ends with a semicolon. The whole line of text is additionally added to
 * the history to allow for convenient browsing of past lines.
 *
 * @return  The line of text.
 */
std::string readline();

/** Clear all input. */
void clear_input();

} /* namespace dbshell */

#endif /* READLINE_HPP */
