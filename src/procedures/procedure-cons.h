/**
 * Scheme built-in procedure "cons".
 * Return a Scheme pair with supplied first and second elements.
 */

#ifndef __SCHEME_PROCEDURE_CONS_H__
#define __SCHEME_PROCEDURE_CONS_H__

#include "scheme-procedure.h"

#define PROCEDURE_CONS_NAME "cons"

/**
 * Get Scheme procedure "cons".
 *
 * @return Scheme procedure "cons".
 */
scheme_procedure *scheme_procedure_cons();

#endif
