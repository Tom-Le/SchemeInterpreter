/**
 * Scheme built-in procedure "cadr".
 *
 * Return the first element of the second pair of a pair.
 */

#ifndef __SCHEME_PROCEDURE_CADR_H__
#define __SCHEME_PROCEDURE_CADR_H__

#include "scheme-procedure.h"

#define PROCEDURE_CADR_NAME "cadr"

/**
 * Get Scheme procedure "cadr".
 */
scheme_procedure *scheme_procedure_cadr();

#endif
