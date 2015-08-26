/**
 * Scheme built-in procedure "number?".
 * Check if an element is a number.
 */

#ifndef __SCHEME_PROCEDURE_ISNUMBER_H__
#define __SCHEME_PROCEDURE_ISNUMBER_H__

#include "scheme-procedure.h"

#define PROCEDURE_ISNUMBER_NAME "number?"

/**
 * Get Scheme procedure "number?".
 *
 * @return Scheme procedure "number?".
 */
scheme_procedure *scheme_procedure_get();

#endif

