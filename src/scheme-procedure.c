#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scheme-procedure.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

/**** Private function declarations ****/

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

/**
 * Compare a Scheme procedure to another procedure.
 * Will return 0 if either pointer is not a procedure.
 *
 * @param  element  Should be a Scheme procedure.
 * @param  other    A Scheme element.
 *
 * @return 1 if equal, 0 otherwise.
 */
static int _vtable_compare(scheme_element *element, scheme_element *other);

/**** Variable initializations ****/

// Global virtual function table.
static struct scheme_element_vtable _scheme_procedure_vtable = {
    .get_type = scheme_procedure_get_type,
    .free = _vtable_free,
    .copy = _vtable_copy,
    .print = _vtable_print,
    .compare = _vtable_compare
};

// Static struct for namespace's type.
static struct scheme_element_type _scheme_procedure_type = {
    .super = NULL,
    .name = "scheme_procedure"
};
static int _scheme_procedure_type_initd = 0;

/**** Private function implementations ****/

static void _vtable_free(scheme_element *element)
{
    scheme_procedure *procedure = (scheme_procedure *)element;

    free(procedure->name);
    free(procedure);
}

static void _vtable_print(scheme_element *element)
{
    scheme_procedure *procedure = (scheme_procedure *)element;

    if (procedure->name != NULL)
        printf("#<procedure:%s>", procedure->name);
    else
        printf("#<procedure>");
}

static scheme_element *_vtable_copy(scheme_element *element)
{
    scheme_procedure *procedure = (scheme_procedure *)element;

    // Create new copy.
    scheme_procedure *procCopy = malloc(sizeof(scheme_procedure));
    if (procCopy == NULL)
        return NULL;

    // Initialize copy.
    scheme_procedure_init(procCopy, procedure->name, procedure->function);

    return (scheme_element *)procCopy;
}

static int _vtable_compare(scheme_element *element, scheme_element *other)
{
    if (!scheme_element_is_type(other, &_scheme_procedure_type)) return 0;

    scheme_procedure *this = (scheme_procedure *)element;
    scheme_procedure *that = (scheme_procedure *)other;

    if (this->name != NULL || that->name != NULL)
    {
        if (this->name == NULL && that->name != NULL) return 0;
        if (this->name != NULL && that->name == NULL) return 0;

        if (strcmp(this->name, that->name) != 0) return 0;
    }

    if (this->function != that->function) return 0;

    return 1;
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

scheme_element *scheme_procedure_apply(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    if (procedure->function == NULL) return NULL;
    return procedure->function(procedure, element, namespace);
}

/**** Implementations of public functions from scheme-procedure-init.h ****/

void scheme_procedure_init(scheme_procedure *proc, char *name, scheme_procedure_function_t function)
{
    // Initialize vtable.
    proc->super.vtable = &_scheme_procedure_vtable;

    // Copy name.
    if (name != NULL)
    {
        char *nameBuf;
        int bufLen = strlen(name) + 1;
        if ((nameBuf = malloc(sizeof(char) * bufLen)) == NULL)
        {
            proc->name = NULL;
            return;
        }
        strcpy(nameBuf, name);

        proc->name = nameBuf;
    }
    else
        proc->name = NULL;

    // Copy function.
    proc->function = function;
}

scheme_element_type *scheme_procedure_get_type()
{
    if (!_scheme_procedure_type_initd)
    {
        _scheme_procedure_type.super = scheme_element_get_base_type();

        _scheme_procedure_type_initd = 1;
    }

    return &_scheme_procedure_type;
}
