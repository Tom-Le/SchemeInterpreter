/**
 * Scheme built-in procedure "<".
 *
 * Return #t if the arguments in the given order are numbers in
 * strictly decreasing order, #f otherwise.
 */

#ifndef __SCHEME_PROCEDURE_LESS_H__
#define __SCHEME_PROCEDURE_LESS_H__

#include "scheme-procedure.h"

#define PROCEDURE_LESS_NAME "<"

/**
 * Get Scheme procedure "<".
 */
scheme_procedure *scheme_procedure_get();

#endif

