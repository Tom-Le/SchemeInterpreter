/**
 * Scheme built-in procedure "lambda".
 * Create an anonymous procedure with Scheme expressions.
 */

#ifndef __SCHEME_PROCEDURE_LAMBDA_H__
#define __SCHEME_PROCEDURE_LAMBDA_H__

#include "scheme-procedure.h"

#define PROCEDURE_LAMBDA_NAME "lambda"

/**
 * Get Scheme procedure "lambda".
 *
 * @return Scheme procedure "lambda".
 */
scheme_procedure *scheme_procedure_get();

#endif
