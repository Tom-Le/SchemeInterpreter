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

#include "scheme-data-types.h"
#include "scheme-element-private.h"
#include "scheme-procedure.h"

/**
 * Typedef for function pointer that can be stored in a Scheme procedure.
 *
 * @param Procedure that stores function pointer.
 * @param A Scheme element to be processed by procedure.
 * @param Active namespace.
 */
typedef scheme_element *(*scheme_procedure_function_t)(scheme_procedure *, scheme_element *, scheme_namespace *);

// Scheme procedure struct.
struct scheme_procedure {
    struct scheme_element super;
    char *name;
    scheme_procedure_function_t function;
};

// Global virtual function table.
struct scheme_element_vtable scheme_procedure_vtable;

/**
 * Initialize a scheme_procedure struct that has already been allocated.
 *
 * Must be called when allocating a new scheme_procedure struct, whether
 * dynamically or statically.
 *
 * If a Scheme procedure has been dynamically allocated, it must be freed
 * with scheme_element_free().
 *
 * The given function pointer has to conform to the function signature
 * defined by the scheme_procedure_function_t typedef. It will be called
 * whenever a procedure is applied on a Scheme expression.
 *
 * @param  proc      A Scheme procedure.
 * @param  name      Proecdure's name. If procedure is unnamed, pass NULL.
 * @param  function  A function pointer.
 */
void scheme_procedure_init(scheme_procedure *proc, char *name, scheme_procedure_function_t function);

#endif
