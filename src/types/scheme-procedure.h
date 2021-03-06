/**
 * Scheme procedure.
 *
 * This type is compatible with scheme_element.
 * You may cast any scheme_procedure pointer to scheme_element and pass it to
 * any function that accepts a scheme_element pointer.
 */

#ifndef __SCHEME_PROCEDURE_H__
#define __SCHEME_PROCEDURE_H__

#include "scheme-data-types.h"

// Scheme procedure.
typedef struct scheme_procedure scheme_procedure;

/**
 * Get a Scheme procedure's name.
 * Returned pointer must be freed with free().
 *
 * @param  proc  A Scheme procedure.
 */
char *scheme_procedure_get_name(scheme_procedure *proc);

/**
 * Apply Scheme procedure on an element.
 *
 * Execute function pointer stored in Scheme procedure with given element
 * as argument to the function. Return what the function returns.
 *
 * Caller must free returned pointer with scheme_element_free().
 *
 * This function itself returns NULL if procedure does not contain any
 * function. The function stored in a procedure may return NULL as
 * necessary, usually to indicate that it could not evaluate an expression.
 *
 * @param  procedure  A Scheme procedure.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Result of applying procedure, or NULL if procedure contains
 *   no function.
 */
scheme_element *scheme_procedure_apply(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Get procedure's type.
 *
 * @return Procedure's type.
 */
scheme_element_type *scheme_procedure_get_type();

#endif
