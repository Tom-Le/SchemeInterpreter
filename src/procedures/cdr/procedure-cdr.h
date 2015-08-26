/**
 * Scheme built-in procedure "cdr".
 * Returns the second element of a Scheme pair.
 */

#ifndef __SCHEME_PROCEDURE_CDR_H__
#define __SCHEME_PROCEDURE_CDR_H__

#include "scheme-procedure.h"

#define PROCEDURE_CDR_NAME "cdr"

/**
 * Get Scheme procedure "cdr".
 *
 * @return Scheme procedure "cdr".
 */
scheme_procedure *scheme_procedure_get();

#endif
