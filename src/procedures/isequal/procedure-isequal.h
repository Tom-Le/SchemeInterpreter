/*
 * Scheme built-in procedure "equal??".
 * Compare two Scheme elements.
 */

#ifndef __SCHEME_PROCEDURE_EQUAL_H__
#define __SCHEME_PROCEDURE_EQUAL_H__

#include "scheme-procedure.h"

#define PROCEDURE_EQUAL_NAME "equal?"

/**
 * @return Scheme procedure "equal?".
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
