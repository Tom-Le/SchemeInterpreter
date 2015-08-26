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
scheme_procedure *scheme_procedure_get();

/**
 * @return Number of aliases for this procedure.
 */
int scheme_procedure_get_alias_count();

/**
 * @param  index  Alias' index.
 *
 * @return Alias at specified index.
 */
const char *scheme_procedure_get_alias(int index);

#endif
