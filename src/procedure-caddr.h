/**
 * Scheme built-in procedure "caddr".
 *
 * Return the first element of the second pair of the second pair
 * of a pair.
 */

#ifndef __SCHEME_PROCEDURE_CADDR_H__
#define __SCHEME_PROCEDURE_CADDR_H__

#include "scheme-procedure.h"

#define PROCEDURE_CADDR_NAME "caddr"

/**
 * Get Scheme procedure "caddr".
 */
scheme_procedure *scheme_procedure_caddr();

#endif
