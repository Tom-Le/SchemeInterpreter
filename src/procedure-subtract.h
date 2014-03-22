/**
 * Scheme built-in procedure "-".
 *
 * Given a number, negate it.
 *
 * Given a list of numbers, return the first number subtracted by the rest.
 */

#ifndef __SCHEME_PROCEDURE_SUBTRACT_H__
#define __SCHEME_PROCEDURE_SUBTRACT_H__

#include "scheme-procedure.h"

#define PROCEDURE_SUBTRACT_NAME "-"

/**
 * Get Scheme procedure "-".
 */
scheme_procedure *scheme_procedure_subtract();

#endif
