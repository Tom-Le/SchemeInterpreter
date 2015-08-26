/**
 * Scheme built-in procedure ">=".
 *
 * Return #t if the arguments in the given order are numbers in
 * non-decreasing order, #f otherwise.
 */

#ifndef __SCHEME_PROCEDURE_GREATEREQUAL_H__
#define __SCHEME_PROCEDURE_GREATEREQUAL_H__

#include "scheme-procedure.h"

#define PROCEDURE_GREATEREQUAL_NAME ">="

/**
 * Get Scheme procedure ">=".
 */
scheme_procedure *scheme_procedure_get();

#endif

