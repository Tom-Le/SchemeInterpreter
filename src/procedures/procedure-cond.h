/**
 * Scheme built-in procedure "cond".
 *
 *    (cond (<condition> [<expression>]) ...)
 *
 * Return the result of the first <expression> whose <condition> does not
 * evaluate to #f. If there is no <expression>, returns <condition>.
 *
 * If all <condition> evaluates to #f, returns void symbol.
 */

#ifndef __SCHEME_PROCEDURE_COND_H__
#define __SCHEME_PROCEDURE_COND_H__

#include "scheme-procedure.h"

#define PROCEDURE_COND_NAME "cond"

/**
 * Get Scheme procedure "cond".
 *
 * @return Scheme procedure "cond".
 */
scheme_procedure *scheme_procedure_cond();

#endif
