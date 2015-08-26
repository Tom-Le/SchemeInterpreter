/**
 * Scheme built-in procedure "append".
 * Append an element onto the end of a list.
 */

#ifndef __SCHEME_PROCEDURE_APPEND_H__
#define __SCHEME_PROCEDURE_APPEND_H__

#include "scheme-procedure.h"

#define PROCEDURE_APPEND_NAME "append"

/**
 * Get Scheme procedure "append".
 *
 * @return Scheme procedure "append".
 */
scheme_procedure *scheme_procedure_get();

#endif
