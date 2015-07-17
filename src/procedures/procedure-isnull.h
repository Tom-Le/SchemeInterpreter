/**
 * Scheme built-in procedure "null?".
 * Check if an element is the empty pair.
 */

#ifndef __SCHEME_PROCEDURE_NULL_H__
#define __SCHEME_PROCEDURE_NULL_H__

#include "scheme-procedure.h"

#define PROCEDURE_NULL_NAME "null?"

/**
 * Get Scheme procedure "null?".
 *
 * @return Scheme procedure "null?".
 */
scheme_procedure *scheme_procedure_isnull();

#endif
