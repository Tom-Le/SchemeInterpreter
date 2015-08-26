/**
 * Scheme built-in procedure "if".
 *
 *     (if <test-expr> <then-expr> <else-expr>)
 *
 * Takes a test expression and two additional expressions. If test expression
 * evaluates to anything other than #f, first out of the two additional
 * expressions is evaluated and returned. Otherwise, second expression is
 * evaluated and returned.
 */

#ifndef __SCHEME_PROCEDURE_IF_H__
#define __SCHEME_PROCEDURE_IF_H__

#include "scheme-procedure.h"

#define PROCEDURE_IF_NAME "if"

/**
 * Get Scheme procedure "if".
 *
 * @return Scheme procedure "if".
 */
scheme_procedure *scheme_procedure_get();

#endif
