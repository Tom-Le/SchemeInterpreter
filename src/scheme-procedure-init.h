/**
 * Functions for initializing a Scheme procedure.
 *
 * These functions are implemented in scheme-procedure.c.
 */

#ifndef __SCHEME_PROCEDURE_PRIVATE_H__
#define __SCHEME_PROCEDURE_PRIVATE_H__

#include "scheme-element.h"
#include "scheme-element-private.h"
#include "scheme-procedure.h"

// Function that can be stored in a Scheme procedure.
typedef scheme_element *(*scheme_procedure_function_t)(scheme_element *);

// Scheme procedure.
struct scheme_procedure {
    struct scheme_element super;
    char *name;
    scheme_procedure_function_t function;
};

/**
 * Initialize a static scheme_procedure struct.
 *
 * Must be called when allocating a new scheme_procedure struct,
 * dynamically or statically.
 *
 * If scheme_procedure was dynamically allocated, it can be
 * freed with scheme_element_free().
 *
 * @param  proc  A Scheme procedure.
 */
void scheme_procedure_init(scheme_procedure *proc, char *name, scheme_procedure_function_t function);

#endif
