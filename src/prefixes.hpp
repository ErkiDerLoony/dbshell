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
   * Create a new prefix mapping with the following initial default prefixes:
   * <ul>
   * <li>“rdf:” for <a href=http://www.w3.org/1999/02/22-rdf-syntax-ns#>http://www.w3.org/1999/02/22-rdf-syntax-ns#</a>
   * <li>“rdfs:” for <a href=http://www.w3.org/2000/01/rdf-schema#>http://www.w3.org/2000/01/rdf-schema#</a>
   * <li>“xsd:” for <a href=http://www.w3.org/2001/XMLSchema#>http://www.w3.org/2001/XMLSchema#</a>
   * <li>“dc:” for <a href=http://purl.org/dc/terms/>http://purl.org/dc/terms/</a>
   * <li>“owl:” for <a href=http://www.w3.org/2002/07/owl#>http://www.w3.org/2002/07/owl#</a>
   * <li>“dbpedia:” for <a href=http://dbpedia.org/>http://dbpedia.org/</a>
   * <li>“de.dbpedia:” for <a href=http://de.dbpedia.org/>http://de.dbpedia.org/</a>
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
   * Format this mapping’s prefixes ready for inclusion in SPARQL queries.
   *
   * @return  A string that contains this mapping’s prefixes formatted like
   *          “PREFIX : <http://example.org>”.
   */
  virtual std::wstring format() const;

  /**
   * Format this mapping’s prefixes into a table.
   *
   * @return  A table that contains this mapping’s prefixes.
   */
  virtual std::unique_ptr<table> format_as_table() const;

private:

  std::map<std::wstring, std::wstring> _prefixes;

}; /* class prefixes */

} /* namespace dbshell */

#endif /* PREFIXES_HPP */
