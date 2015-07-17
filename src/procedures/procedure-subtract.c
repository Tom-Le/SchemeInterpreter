#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-subtract.h"

/**** Private variables ****/

static scheme_procedure _procedure_subtract;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "-".
 *
 * Given a number, negate it.
 * Given a list of numbers, return the first number subtracted by the rest.
 *
 * Will return NULL if:
 * - Element is not a list in the format: (<number> ...).
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Result of calculation or NULL if an error occurs.
 */
static scheme_element *_subtract_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_subtract_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    // Evaluate each argument.
    scheme_pair *evaluatedList = scheme_list_evaluated((scheme_pair *)element, namespace);
    if (evaluatedList == NULL)
    {
        return NULL;
    }

    // Get arguments.
    int argCount;
    scheme_element **arguments = scheme_list_to_array(evaluatedList, &argCount);

    // Terminate if element is not a list or is the empty list.
    if (argCount == -1 || argCount == 0)
    {
        return NULL;
    }

    // Verify that every argument is a number.
    for (int i = 0; i < argCount; ++i)
    {
        scheme_element *anArgument = arguments[i];

        if (!scheme_element_is_type(anArgument, scheme_number_get_type()))
        {
            free(arguments);
            scheme_element_free((scheme_element *)evaluatedList);
            return NULL;
        }
    }

    long result;
    if (argCount == 1)
    {
        // If there is only one argument, negate it.
        result = -scheme_number_get_value((scheme_number *)arguments[0]);
    }
    else
    {
        // If there are more than one arguments, get the first one and
        // subtract it by the rest.

        result = scheme_number_get_value((scheme_number *)arguments[0]);
        for (int i = 1; i < argCount; ++i)
        {
            result -= scheme_number_get_value((scheme_number *)arguments[i]);
        }
    }

    free(arguments);
    scheme_element_free((scheme_element *)evaluatedList);
    return (scheme_element *)scheme_number_new(result);
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_subtract()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_subtract, PROCEDURE_SUBTRACT_NAME, _subtract_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_subtract.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_subtract.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_subtract;
}

