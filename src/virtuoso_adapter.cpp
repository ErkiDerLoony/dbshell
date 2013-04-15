#include "virtuoso_adapter.hpp"
#include "virtuoso_defines.hpp"
#include "log.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

#include <sql.h>
#include <sqlext.h>

using namespace dbshell;
using std::unique_ptr;
using std::string;
using std::cerr;
using std::cout;
using std::endl;
using std::runtime_error;
using std::stringstream;
using std::ifstream;

unique_ptr<string> virtuoso_adapter::pwd(const std::string& username) {
  ifstream file("/home/edgar/.vtpass");

  if (file.is_open()) {
    string line;

    while (file.good()) {
      getline(file, line);

      if (line.substr(0, username.length()) == username) {
        line = line.substr(username.length());

        if (line.substr(0, 1) == " ") {
          line = line.substr(1);
        }

        return unique_ptr<string>(new string(line));
      }
    }
  }

  return unique_ptr<string>(nullptr);
}

virtuoso_adapter::virtuoso_adapter(std::string host, std::string username) throw(runtime_error) {
  log::debug << "Creating virtuoso adapter." << endl;
  log::debug << "Allocating environment." << endl;

  if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &environment))) {
    throw runtime_error("Error creating environment!");
  }

  log::debug << "Adapting ODBC version." << endl;

  if (!SQL_SUCCEEDED(SQLSetEnvAttr(environment, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0))) {
    throw runtime_error("Error setting ODBC version!");
  }

  log::debug << "Creating connection handle." << endl;

  if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_DBC, environment, &connection))) {
    throw runtime_error("Error creating connection!");
  }

  log::debug << "Loading password." << endl;
  unique_ptr<string> pointer = pwd(username);
  string password;

  if (pointer == nullptr) {
    log::warning << "WARNING: No password for user »" << username << "« found in ~/.vtpass, this will likely NOT work!" << endl;
  } else {
    password = *pointer;
    log::debug << "Got password »" << password << "«." << endl;
  }

  string dsn = "saruman";
  log::debug << "Connecting to data source »" << dsn << "«." << endl;

  if (!SQL_SUCCEEDED(SQLConnect(connection, (SQLCHAR*) dsn.c_str(), SQL_NTS, (SQLCHAR*) username.c_str(), SQL_NTS, (SQLCHAR*) password.c_str(), SQL_NTS))) {
    throw runtime_error("Error connecting to data source!");
  }

  log::debug << "Allocating statement." << endl;

  if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, connection, &statement))) {
    throw runtime_error("Error allocating statement!");
  }
}

virtuoso_adapter::~virtuoso_adapter() {

  if (statement) {
    log::debug << "Releasing statement." << endl;
    SQLFreeHandle(SQL_HANDLE_STMT, statement);
  }

  if (connection) {
    log::debug << "Disconnecting and releasing connection." << endl;
    SQLDisconnect(connection);
    SQLFreeHandle(SQL_HANDLE_DBC, connection);
  }

  if (environment) {
    log::debug << "Releasing environment." << endl;
    SQLFreeHandle(SQL_HANDLE_ENV, environment);
  }

  log::debug << "Virtuoso adapter destroyed." << endl;
}

unique_ptr<table> virtuoso_adapter::query(string query) throw (runtime_error) {
  unique_ptr<table> table(unique_ptr<dbshell::table>(new dbshell::table()));

  if (SQLExecDirect(statement, (SQLCHAR*) query.c_str(), SQL_NTS) != SQL_SUCCESS) {
    throw runtime_error("Error executing query!");
  }

  char fetchBuffer[1000];
  short numCols = 0;
  short colNum;
  SQLLEN colIndicator;
  UDWORD totalRows;
  UDWORD totalSets;
  SQLHANDLE hdesc = SQL_NULL_HANDLE;
  SQLRETURN rc;

  totalSets = 0;

  do {
    rc = SQLNumResultCols (statement, &numCols);

    if (!SQL_SUCCEEDED (rc)) {
      SQLCloseCursor(statement);
      throw runtime_error("Could not get number of result columns!");
    }

    if (numCols == 0) {
      cout << "Statement executed." << endl;
      return table;
    }

    totalRows = 0;

    while (1) {
      rc = SQLFetch(statement);

      if (rc == SQL_NO_DATA_FOUND) {
        break;
      }

      if (!SQL_SUCCEEDED(rc)) {
        SQLCloseCursor(statement);
        throw runtime_error("Could not fetch any data!");
      }

      for (colNum = 1; colNum <= numCols; colNum++) {
        char buf[1000];
        SQLINTEGER len;
        int flag, dvtype;

        rc = SQLGetData(statement, colNum, SQL_C_CHAR, fetchBuffer, sizeof(fetchBuffer), &colIndicator);

        if (!SQL_SUCCEEDED (rc)) {
          SQLCloseCursor(statement);
          throw runtime_error("Could not get column data!");
        }

        rc = SQLGetStmtAttr(statement, SQL_ATTR_IMP_ROW_DESC, &hdesc, SQL_IS_POINTER, NULL);

        if (!SQL_SUCCEEDED (rc)) {
          SQLCloseCursor(statement);
          throw runtime_error("Could not create column attribute handle!");
        }

        rc = SQLGetDescField (hdesc, colNum, SQL_DESC_COL_DV_TYPE, &dvtype, SQL_IS_INTEGER, NULL);

        if (!SQL_SUCCEEDED (rc)) {
          SQLCloseCursor(statement);
          throw runtime_error("Could not get column data type!");
        }

        rc = SQLGetDescField (hdesc, colNum, SQL_DESC_COL_BOX_FLAGS, &flag, SQL_IS_INTEGER, NULL);

        if (!SQL_SUCCEEDED (rc)) {
          SQLCloseCursor(statement);
          throw runtime_error("Could not get column flags!");
        }

        if (colIndicator == SQL_NULL_DATA) {
          printf ("NULL");
        } else {

          if (flag & VIRTUOSO_BF_IRI)
            printf ("<%s>", fetchBuffer); /* IRI */
          else if (dvtype == VIRTUOSO_DV_STRING || dvtype == VIRTUOSO_DV_RDF)
            printf ("\"%s\"", fetchBuffer); /* literal string */
          else
            printf ("%s", fetchBuffer); /* value */

          if (dvtype == VIRTUOSO_DV_RDF) {
            rc = SQLGetDescField(hdesc, colNum, SQL_DESC_COL_LITERAL_LANG, buf, sizeof (buf), &len);

            if (!SQL_SUCCEEDED(rc)) {
              SQLCloseCursor(statement);
              throw runtime_error("Could not get literal language!");
            }

            if (len) {
              printf ("@%.*s", (int) len, buf);
            }

            rc = SQLGetDescField(hdesc, colNum, SQL_DESC_COL_LITERAL_TYPE, buf, sizeof (buf), &len);

            if (!SQL_SUCCEEDED (rc)) {
              SQLCloseCursor(statement);
              throw runtime_error("Could not get literal type!");
            }

            if (len) {
              printf ("^^<%.*s>", (int) len, buf);
            }
          }

          if (colNum < numCols) {
            putchar (' ');
          }
        }
      }

      printf (" .\n");
      totalRows++;
    }

    printf ("\n\nStatement returned %du rows.\n", totalRows);
    totalSets++;
  } while (SQLMoreResults(statement) == SQL_SUCCESS);

  return table;
}

void virtuoso_adapter::cancel() {
}
