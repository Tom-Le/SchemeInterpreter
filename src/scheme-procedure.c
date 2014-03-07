#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scheme-procedure.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

/**** Private function declarations ****/

/**
 * Initialize static variables.
 */
static void _static_init();

/**
 * Return procedure's type identifier.
 *
 * @param  element  Should be a Scheme procedure.
 *
 * @return Type identifier.
 */
static char *_vtable_get_type(scheme_element *element);

/**
 * Free Scheme procedure.
 *
 * @param  element  Should be a Scheme procedure.
 */
static void _vtable_free(scheme_element *element);

/**
 * Print Scheme procedure to stdout.
 *
 * @param  element  Should be a Scheme procedure.
 */
static void _vtable_print(scheme_element *element);

/**
 * Copy a procedure.
 * Returned pointer must be freed with scheme_element_free().
 *
 * @param  element  Should be a Scheme procedure.
 *
 * @return Copy or NULL if out of memory.
 */
static scheme_element *_vtable_copy(scheme_element *element);

/**** Private variables ****/

// Global virtual function table.
static struct scheme_element_vtable _scheme_procedure_vtable;

static int _static_initialized = 0;

/**** Private function implementations ****/

static void _static_init()
{
    if (!_static_initialized)
    {
        _scheme_procedure_vtable.get_type = _vtable_get_type;
        _scheme_procedure_vtable.free = _vtable_free;
        _scheme_procedure_vtable.print = _vtable_print;
        _scheme_procedure_vtable.copy = _vtable_copy;

        _static_initialized = 1;
    }
}

static char *_vtable_get_type(scheme_element *element)
{
    return SCHEME_PROCEDURE_TYPE;
}

static void _vtable_free(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_PROCEDURE_TYPE))
        return;

    scheme_procedure *procedure = (scheme_procedure *)element;

    free(procedure->name);
    free(procedure);
}

static void _vtable_print(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_PROCEDURE_TYPE))
        return;

    scheme_procedure *procedure = (scheme_procedure *)element;

    printf("#<procedure:%s>", procedure->name);
}

static scheme_element *_vtable_copy(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_PROCEDURE_TYPE))
        return NULL;

    scheme_procedure *procedure = (scheme_procedure *)element;

    // Create new copy.
    scheme_procedure *procCopy = malloc(sizeof(scheme_procedure));
    if (procCopy == NULL)
        return NULL;

    // Initialize copy.
    scheme_procedure_init(procCopy, procedure->name, procedure->function);

    return (scheme_element *)procCopy;
}

/**** Implementations of public functions from scheme-procedure.h ****/

char *scheme_procedure_get_name(scheme_procedure *proc)
{
    // Allocate return buffer.
    char *returnBuf;
    int bufLen = strlen(proc->name) + 1;
    if ((returnBuf = malloc(sizeof(char) * bufLen)) == NULL)
        return NULL;

    strcpy(returnBuf, proc->name);
    return returnBuf;
}

scheme_element *scheme_procedure_apply(scheme_procedure *proc, scheme_element *elem)
{
    if (proc->function == NULL) return NULL;
    return proc->function(elem);
}

/**** Implementations of public functions from scheme-procedure-init.h ****/

void scheme_procedure_init(scheme_procedure *proc, char *name, scheme_procedure_function_t function)
{
    _static_init();

    // Initialize vtable.
    proc->super.vtable = &_scheme_procedure_vtable;

    // Copy name.
    char *nameBuf;
    int bufLen = strlen(name) + 1;
    if ((nameBuf = malloc(sizeof(char) * bufLen)) == NULL)
    {
        proc->name = NULL;
        return;
    }
    strcpy(nameBuf, name);

    proc->name = nameBuf;

    // Copy function.
    proc->function = function;
}
