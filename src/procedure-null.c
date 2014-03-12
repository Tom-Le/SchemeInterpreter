#include <stdlib.h>
#include <string.h>

#include "eval.h"
#include "procedure-utils.h"
#include "scheme-data-types.h"
#include "scheme-procedure-init.h"

#include "procedure-null.h"

/**** Private variables ****/

static scheme_procedure _procedure_null;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "null?".
 * Check if a Scheme element is the empty pair.
 *
 * Will return NULL if:
 * - Supplied element is not a pair in the format (<element>).
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Scheme boolean #t if two elements are equal, #f if not, or NULL
 *         if an error occurs.
 */
static scheme_element *_null_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_null_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
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
    arg = scheme_evaluate(arg, namespace);
    if (arg == NULL) return NULL;

    int isEmpty = scheme_element_is_type(arg, SCHEME_PAIR_TYPE)
               && scheme_pair_is_empty((scheme_pair *)arg);
    scheme_element_free(arg);

    return (scheme_element *)(isEmpty ? scheme_boolean_get_true() : scheme_boolean_get_false());
}

/**** Public function implementation ****/
scheme_procedure *scheme_procedure_null()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_null, PROCEDURE_NULL_NAME, _null_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_null.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_null.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_null;
}
