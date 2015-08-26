/**
 * Scheme built-in procedure "assoc".
 * Find a pair among a list of pairs that is associated with a given element,
 * or #f if the given element does not associate with any pair in the list.
 */

#ifndef __SCHEME_PROCEDURE_ASSOC_H__
#define __SCHEME_PROCEDURE_ASSOC_H__

#include "scheme-procedure.h"

#define PROCEDURE_ASSOC_NAME "assoc"

/**
 * Get Scheme procedure "assoc".
 *
 * @return Scheme procedure "assoc".
 */
scheme_procedure *scheme_procedure_get();

#endif
