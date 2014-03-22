/**
 * Scheme built-in procedure "symbol?".
 * Check if an element is a symbol.
 */

#ifndef __SCHEME_PROCEDURE_ISSYMBOL_H__
#define __SCHEME_PROCEDURE_ISSYMBOL_H__

#include "scheme-procedure.h"

#define PROCEDURE_ISSYMBOL_NAME "symbol?"

/**
 * Get Scheme procedure "symbol?".
 *
 * @return Scheme procedure "symbol?".
 */
scheme_procedure *scheme_procedure_issymbol();

#endif
