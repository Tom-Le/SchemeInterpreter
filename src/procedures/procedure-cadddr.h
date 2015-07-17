/**
 * Scheme built-in procedure "cadddr".
 *
 * Return the first element of the second pair of the second pair
 * of the second pair of a pair.
 */

#ifndef __SCHEME_PROCEDURE_CADDDR_H__
#define __SCHEME_PROCEDURE_CADDDR_H__

#include "scheme-procedure.h"

#define PROCEDURE_CADDDR_NAME "cadddr"

/**
 * Get Scheme procedure "cadddr".
 */
scheme_procedure *scheme_procedure_cadddr();

#endif
