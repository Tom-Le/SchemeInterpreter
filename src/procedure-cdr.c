#include <stdlib.h>

#include "eval.h"
#include "procedure-utils.h"
#include "scheme-data-types.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-cdr.h"

/**** Private variables ****/

static scheme_procedure _procedure_cdr;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "cdr".
 * Return the second element of the Scheme pair supplied as argument.
 *
 * Will return NULL if:
 * - Supplied element is not a pair in the format: (<pair>)
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Second element, or NULL if an error occurred.
 */
static scheme_element *_cdr_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_cdr_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    // Get arguments.
    int argCount;
    scheme_element **args = procedure_get_arguments(element, &argCount);

    // Check if argument list is invalid.
    if (argCount == -1) return NULL;
    else if (argCount != 1)
    {
        // Wrong number of arguments.
        if (args != NULL) free(args);
        return NULL;
    }

    scheme_element *arg = *args;
    free(args);

    // Evaluate argument.
    scheme_element *result = scheme_evaluate(arg, namespace);
    if (result == NULL) return NULL;

    // Evaluated result must be a pair.
    if (!scheme_element_is_type(result, SCHEME_PAIR_TYPE))
    {
        scheme_element_free(result);
        return NULL;
    }

    // Get a copy of the second element of result.
    scheme_element *cdr = scheme_pair_get_second((scheme_pair *)result);
    cdr = scheme_element_copy(cdr);

    scheme_element_free(result);

    return cdr;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_cdr()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_cdr, PROCEDURE_CDR_NAME, _cdr_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_cdr.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_cdr.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_cdr;
}
