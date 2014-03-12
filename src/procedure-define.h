/**
 * Scheme built-in procedure "define".
 * Associate an identifier with a Scheme element in current namespace.
 */

#ifndef __SCHEME_PROCEDURE_DEFINE_H__
#define __SCHEME_PROCEDURE_DEFINE_H__

#include "scheme-procedure.h"

#define PROCEDURE_DEFINE_NAME "define"

/**
 * Get Scheme procedure "define".
 *
 * @return Scheme procedure "define".
 */
scheme_procedure *scheme_procedure_define();

#endif
