#ifndef PREFIXES_HPP
#define PREFIXES_HPP

#include "table.hpp"

#include <string>
#include <map>
#include <memory>

namespace dbshell {

class prefixes {

public:

  /**
   * Create a new prefix mapping that contains---among others---the following initial prefixes:
   * <ul>
   * <li>“rdf:” for <a href=http://www.w3.org/1999/02/22-rdf-syntax-ns#>http://www.w3.org/1999/02/22-rdf-syntax-ns#</a>
   * <li>“rdfs:” for <a href=http://www.w3.org/2000/01/rdf-schema#>http://www.w3.org/2000/01/rdf-schema#</a>
   * <li>“xsd:” for <a href=http://www.w3.org/2001/XMLSchema#>http://www.w3.org/2001/XMLSchema#</a>
   * <li>“dc:” for <a href=http://purl.org/dc/terms/>http://purl.org/dc/terms/</a>
   * <li>“owl:” for <a href=http://www.w3.org/2002/07/owl#>http://www.w3.org/2002/07/owl#</a>
   * </ul>
   */
  prefixes();

  /** Destroy this prefix map and free all allocated resources. */
  virtual ~prefixes();

  /**
   * Apply this mapping’s prefixes to the values in a given table.
   *
   * @param t  The table whose values shall be prefixed.
   */
  virtual void prefix(table& t) const;

  /**
   * Apply this prefix map to one value.
   *
   * @param t  The value that shall be prefixed.
   * @return  The prefixed value which may equal the specified value if no
   *          prefix of this mapping applied.
   */
  virtual std::string prefix(const std::string& t) const;

  /**
   * Format this mapping’s prefixes ready for inclusion in SPARQL queries.
   *
   * @return  A string that contains this mapping’s prefixes formatted like
   *          “PREFIX : <http://example.org>”.
   */
  virtual std::string format() const;

  /**
   * Add a prefix to this mapping.
   *
   * @param prefix  The new prefix.
   * @param iri     The full iri of the new prefix.
   */
  virtual void add(std::string prefix, std::string iri);

  /**
   * Check whether or not this mapping contains a specific prefix.
   *
   * @param prefix  The prefix to check for.
   * @return        {@code true} if this mapping contains the given prefix or
   *                {@code false} otherwise.
   */
  virtual bool contains(std::string prefix);

  /**
   * Remove a prefix from this mapping.
   *
   * @param prefix  The prefix that shall be removed.
   */
  virtual void remove(std::string prefix);

  /**
   * Format this mapping’s prefixes into a table.
   *
   * @return  A table that contains this mapping’s prefixes.
   */
  virtual std::unique_ptr<table> format_as_table() const;

private:

  std::map<std::string, std::string> _prefixes;

}; /* class prefixes */

} /* namespace dbshell */

#endif /* PREFIXES_HPP */
