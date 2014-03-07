/**
 * Functions for creating and initializing a Scheme procedure.
 *
 * This header file should only be included by code declaring
 * a built-in Scheme procedure.
 *
 * These functions are implemented in scheme-procedure.c.
 */

#ifndef __SCHEME_PROCEDURE_PRIVATE_H__
#define __SCHEME_PROCEDURE_PRIVATE_H__

#include "scheme-element.h"
#include "scheme-element-private.h"
#include "scheme-procedure.h"

// Typedef for function pointer that can be stored in a Scheme procedure.
typedef scheme_element *(*scheme_procedure_function_t)(scheme_element *);

// Scheme procedure struct.
struct scheme_procedure {
    struct scheme_element super;
    char *name;
    scheme_procedure_function_t function;
};

/**
 * Initialize an already-initialized scheme_procedure struct.
 *
 * Must be called when allocating a new scheme_procedure struct,
 * whether dynamically or statically.
 *
 * If a Scheme procedure has been dynamically allocated, it must
 * be freed with scheme_element_free().
 *
 * The given function pointer has to conform to the function
 * signature defined by the scheme_procedure_function_t typedef.
 * It will be called whenever a procedure is applied on a
 * Scheme expression.
 *
 * @param  proc  A Scheme procedure.
 * @param  name  Its name.
 * @param  function  A function pointer.
 */
void scheme_procedure_init(scheme_procedure *proc, char *name, scheme_procedure_function_t function);

#endif
