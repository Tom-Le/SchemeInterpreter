/**
 * Scheme built-in procedure "procedure?".
 * Check if an element is a procedure.
 */

#ifndef __SCHEME_PROCEDURE_ISPROCEDURE_H__
#define __SCHEME_PROCEDURE_ISPROCEDURE_H__

#include "scheme-procedure.h"

#define PROCEDURE_ISPROCEDURE_NAME "procedure?"

/**
 * Get Scheme procedure "procedure?".
 *
 * @return Scheme procedure "procedure?".
 */
scheme_procedure *scheme_procedure_isprocedure();

#endif
