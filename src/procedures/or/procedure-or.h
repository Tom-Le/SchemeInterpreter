/**
 * Scheme built-in procedure "or".
 *
 * Evaluate arguments sequentially and as soon as an argument evaluates
 * to anything other than null (as determined by "null?"), return the
 * result of evaluating that argument.
 *
 * If every argument evaluates to null, return the result of evaluating
 * the last one.
 *
 * If there is no argument, returns #f.
 */

#ifndef __SCHEME_PROCEDURE_OR_H__
#define __SCHEME_PROCEDURE_OR_H__

#include "scheme-procedure.h"

#define PROCEDURE_OR_NAME "or"

/**
 * Get Scheme procedure "or".
 */
scheme_procedure *scheme_procedure_get();

#endif
