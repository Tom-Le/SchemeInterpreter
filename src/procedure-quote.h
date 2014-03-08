/**
 * Scheme built-in procedure "quote".
 * Return the given element as-is.
 */

#ifndef __SCHEME_PROCEDURE_QUOTE_H__
#define __SCHEME_PROCEDURE_QUOTE_H__ 

#include "scheme-procedure.h"

#define PROCEDURE_QUOTE_NAME "quote"

/**
 * Get Scheme procedure "quote".
 *
 * @return Scheme procedure "quote".
 */
scheme_procedure *scheme_procedure_quote();

#endif
