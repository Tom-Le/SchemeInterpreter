/**
 * Scheme built-in procedure "and".
 *
 * Evaluate arguments sequentially and return #f as soon as an
 * argument evaluates to #f. If no argument evaluates to #f,
 * return the result of evaluating the last argument.
 *
 * If there is no argument, return #t.
 */

#ifndef __SCHEME_PROCEDURE_AND_H__
#define __SCHEME_PROCEDURE_AND_H__

#include "scheme-procedure.h"

#define PROCEDURE_AND_NAME "and"

/**
 * Get Scheme procedure "and".
 */
scheme_procedure *scheme_procedure_get();

#endif
