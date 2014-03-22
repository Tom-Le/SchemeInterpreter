/**
 * Scheme built-in procedure "length".
 *
 * Return the length of a Scheme list.
 */

#ifndef __SCHEME_PROCEDURE_LENGTH_H__
#define __SCHEME_PROCEDURE_LENGTH_H__

#include "scheme-procedure.h"

#define PROCEDURE_LENGTH_NAME "length"

/**
 * Get procedure "length".
 */
scheme_procedure *scheme_procedure_length();

#endif
