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
