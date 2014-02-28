/**
 * Scheme procedure.
 *
 * This type is compatible with scheme_element.
 * You may pass any scheme_procedure pointer as a scheme_element pointer.
 */

#ifndef __SCHEME_PROCEDURE_H__
#define __SCHEME_PROCEDURE_H__

#include "scheme-element.h"

// Scheme procedure type.
#define SCHEME_PROCEDURE_TYPE "SCHEME_PROCEDURE_TYPE"

// Scheme procedure.
typedef struct scheme_procedure scheme_procedure;

/**
 * Get a Scheme procedure's name.
 *
 * @param  proc  A Scheme procedure.
 */
char *scheme_procedure_get_name(scheme_procedure *proc);

/**
 * Apply Scheme procedure.
 * Execute function stored in Scheme procedure and return the result.
 * Caller must free returned result with scheme_element_free().
 *
 * @param  proc  A Scheme procedure.
 * @param  elem  A Scheme element.
 *
 * @return Result of applying procedure or NULL if procedure contains
 *    no function.
 */
scheme_element *scheme_procedure_apply(scheme_procedure *proc, scheme_element *elem);

#endif
