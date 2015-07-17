/**
 * Scheme built-in procedure "list?".
 * Check if an element is a list.
 */

#ifndef __SCHEME_PROCEDURE_ISLIST_H__
#define __SCHEME_PROCEDURE_ISLIST_H__

#include "scheme-procedure.h"

#define PROCEDURE_ISLIST_NAME "list?"

/**
 * Get Scheme procedure "list?".
 */
scheme_procedure *scheme_procedure_islist();

#endif
