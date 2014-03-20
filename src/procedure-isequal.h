/*
 * Scheme built-in procedure "equal??".
 * Compare two Scheme elements.
 */

#ifndef __SCHEME_PROCEDURE_EQUAL_H__
#define __SCHEME_PROCEDURE_EQUAL_H__

#include "scheme-procedure.h"

#define PROCEDURE_EQUAL_NAME "equal?"

/**
 * Get Scheme procedure "equal?".
 *
 * @return Scheme procedure "equal?".
 */
scheme_procedure *scheme_procedure_isequal();

#endif
