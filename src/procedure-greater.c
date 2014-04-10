#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "scheme-pair-utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-greater.h"

/**** Private variables ****/

static scheme_procedure _procedure_greater;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure ">".
 *
 * Will return NULL if:
 * - Element is not a list in the format: (<number> ...).
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return #t if numbers are in strictly increasing order, #f if not,
 *         or NULL if an error occurs.
 */
static scheme_element *_greater_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_greater_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
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

    // Terminate if there are less than 2 arguments.
    if (argCount < 2)
    {
        free(arguments);
        scheme_element_free((scheme_element *)evaluatedList);
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

    // Check if numbers are in strictly increasing order.
    long min = scheme_number_get_value((scheme_number *)arguments[0]);
    scheme_boolean *result = scheme_boolean_get_true();
    for (int i = 1; i < argCount; ++i)
    {
        long value = scheme_number_get_value((scheme_number *)arguments[i]);
        if (value > min)
        {
            min = value;
        }
        else
        {
            // Found a number out of strictly increasing order.
            result = scheme_boolean_get_false();
            break;
        }
    }

    free(arguments);
    scheme_element_free((scheme_element *)evaluatedList);
    return (scheme_element *)result;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_greater()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_greater, PROCEDURE_GREATER_NAME, _greater_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_greater.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_greater.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_greater;
}

