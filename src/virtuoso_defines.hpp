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

/*
 * Include Virtuoso ODBC extensions for SPARQL result set
 */
#if !defined (SQL_DESC_COL_DV_TYPE)

/*
 * ODBC extensions for SQLGetDescField
 */
# define SQL_DESC_COL_DV_TYPE               1057L
# define SQL_DESC_COL_DT_DT_TYPE            1058L
# define SQL_DESC_COL_LITERAL_ATTR          1059L
# define SQL_DESC_COL_BOX_FLAGS             1060L
# define SQL_DESC_COL_LITERAL_LANG          1061L
# define SQL_DESC_COL_LITERAL_TYPE          1062L

/*
 * Virtuoso - ODBC SQL_DESC_COL_DV_TYPE
 */
# define VIRTUOSO_DV_DATE                   129
# define VIRTUOSO_DV_DATETIME               211
# define VIRTUOSO_DV_DOUBLE_FLOAT           191
# define VIRTUOSO_DV_IRI_ID                 243
# define VIRTUOSO_DV_LONG_INT               189
# define VIRTUOSO_DV_NUMERIC                219
# define VIRTUOSO_DV_RDF                    246
# define VIRTUOSO_DV_SINGLE_FLOAT           190
# define VIRTUOSO_DV_STRING                 182
# define VIRTUOSO_DV_TIME                   210
# define VIRTUOSO_DV_TIMESTAMP              128
# define VIRTUOSO_DV_TIMESTAMP_OBJ          208

/*
 * Virtuoso - ODBC SQL_DESC_COL_DT_DT_TYPE
 */
# define VIRTUOSO_DT_TYPE_DATETIME          1
# define VIRTUOSO_DT_TYPE_DATE              2
# define VIRTUOSO_DT_TYPE_TIME              3

/*
 * Virtuoso - ODBC SQL_DESC_COL_BOX_FLAGS
 */
#define VIRTUOSO_BF_IRI                     0x1
#define VIRTUOSO_BF_UTF8                    0x2
#define VIRTUOSO_BF_DEFAULT_ENC             0x4

#endif /* (SQL_DESC_COL_DV_TYPE) */
