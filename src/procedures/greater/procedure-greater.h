/**
 * Scheme built-in procedure ">".
 *
 * Return #t if the arguments in the given order are numbers in
 * strictly increasing order, #f otherwise.
 */

#ifndef __SCHEME_PROCEDURE_GREATER_H__
#define __SCHEME_PROCEDURE_GREATER_H__

#include "scheme-procedure.h"

#define PROCEDURE_GREATER_NAME ">"

/**
 * Get Scheme procedure ">".
 */
scheme_procedure *scheme_procedure_get();

#endif
