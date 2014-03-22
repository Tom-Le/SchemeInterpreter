#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "scheme-pair-utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-cadr.h"

static scheme_procedure _procedure_cadr;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "cadr".
 *
 * Will return NULL if:
 * - Supplied element is not a pair in the format: (<pair>)
 * - Argument <pair> is improperly formatted.
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Result of evaluation or NULL if an error occurs.
 */
static scheme_element *_cadr_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_cadr_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    // Get arguments.
    int argCount;
    scheme_element **args = scheme_list_to_array((scheme_pair *)element, &argCount);

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
    if (!scheme_element_is_type(result, scheme_pair_get_type()))
    {
        scheme_element_free(result);
        return NULL;
    }

    // Traverse down one level of resulting pair.
    scheme_element *second = result;
    for (int i = 0; i < 1; ++i)
    {
        second = scheme_pair_get_second((scheme_pair *)second);

        if (!scheme_element_is_type(second, scheme_pair_get_type()))
        {
            scheme_element_free(result);
            return NULL;
        }
    }

    // Get first element.
    scheme_element *first = scheme_pair_get_first((scheme_pair *)second);
    first = scheme_element_copy(first);

    scheme_element_free(result);

    return first;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_cadr()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_cadr, PROCEDURE_CADR_NAME, _cadr_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_cadr.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_cadr.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_cadr;
}
