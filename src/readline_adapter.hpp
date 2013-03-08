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
 * The line of text is additionally added to the history which enables
 * convenient browsing of previous input.
 *
 * @return  The line of text.
 */
std::string readline();

} /* namespace dbshell */

#endif /* READLINE_HPP */
