/**
 * This file defines all the error codes used in procedures.
 * Before append new one, check if there has been a resemble or existing
 * definition. In addition you should keep index increasing.
 */
#ifndef ERRORS_RC_H
#define ERRORS_RC_H

#define RC_OK (0)
#define RC_FAILED (1)
#define RC_TYPE_MISMATCH (2)
#define RC_DB_BROKEN (3)
#define RC_OPENFILE (4)
#define RC_INVALID_MAGIC (5)
#define RC_INVALID_FILE_TYPE (6)
#define RC_INVALID_FILE_VERSION (7)
#define RC_NUMBER_OVERFLOW (8)
#define RC_ELEMENT_ID_DUP (9)
#define RC_UNKNOWN_ELEMENT (10)

#define RC_FAILURE(_rc) (_rc)
#define RC_SUCCESS(_rc) !ES_FAILURE(_rc)
#define RC_CHECK_RETURN(_rc) do{ int _rc2 = _rc; if(RC_FAILURE(_rc2)) return _rc2; }while(0)

#endif // ERRORS_RC_H

