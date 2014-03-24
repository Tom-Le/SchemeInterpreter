/**
 * Scheme built-in procedure "let".
 *
 * Define local bindings then evaluate a list of expressions.
 */

#ifndef __SCHEME_PROCEDURE_LET_H__
#define __SCHEME_PROCEDURE_LET_H__

#include "scheme-procedure.h"

#define PROCEDURE_LET_NAME "let"

/**
 * Get built-in procedure "let".
 */
scheme_procedure *scheme_procedure_let();

#endif
