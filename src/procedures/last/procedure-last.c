#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-last.h"

/**** Private variables ****/

static scheme_procedure _procedure_last;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "last".
 *
 * Will return NULL if:
 * - Supplied element is not a pair in the format: (<list>)
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Last element of list, or NULL if an error occurs.
 */
static scheme_element *_last_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_last_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
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

    scheme_element *arg = args[0];
    free(args);

    // Evaluate argument.
    arg = scheme_evaluate(arg, namespace);
    if (arg == NULL) return NULL;

    // Evaluated result must be a pair.
    if (!scheme_element_is_type(arg, scheme_pair_get_type()))
    {
        scheme_element_free(arg);
        return NULL;
    }

    // Get every item in evaluated result and verify that it is a non-empty list.
    int itemCount;
    scheme_element **items = scheme_list_to_array((scheme_pair *)arg, &itemCount);
    if (itemCount == -1 || itemCount == 0)
    {
        scheme_element_free(arg);
        return NULL;
    }

    // Get last element.
    scheme_element *result = scheme_element_copy(items[itemCount - 1]);
    free(items);
    scheme_element_free(arg);

    return result;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_get()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_last, PROCEDURE_LAST_NAME, _last_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_last.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_last.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_last;
}
