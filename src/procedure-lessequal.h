/**
 * Scheme built-in procedure "<=".
 *
 * Return #t if the arguments in the given order are numbers in
 * non-increasing order, #f otherwise.
 */

#ifndef __SCHEME_PROCEDURE_LESSEQUAL_H__
#define __SCHEME_PROCEDURE_LESSEQUAL_H__

#include "scheme-procedure.h"

#define PROCEDURE_LESSEQUAL_NAME "<="

/**
 * Get Scheme procedure "<=".
 */
scheme_procedure *scheme_procedure_lessequal();

#endif


