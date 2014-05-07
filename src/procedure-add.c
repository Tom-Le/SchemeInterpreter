#include <stdlib.h>

#include "eval.h"
#include "utils.h"
#include "scheme-data-types.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-add.h"

/**** Private variables ****/

static scheme_procedure _procedure_add;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "+".
 * Compute the sum of a list of numbers.
 *
 * Will return NULL if:
 * - Element is not a list in the format: (<number> ...).
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Sum of numbers in the list or NULL if an error occurs.
 */
static scheme_element *_add_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_add_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
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

    // Terminate if element is not a list.
    if (argCount == -1)
    {
        return NULL;
    }

    // Return 0 if there is no argument.
    if (argCount == 0)
    {
        return (scheme_element *)scheme_number_new(0);
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

    // Compute sum of all numbers.
    long sum = 0;
    for (int i = 0; i < argCount; ++i)
    {
        sum += scheme_number_get_value((scheme_number *)arguments[i]);
    }

    free(arguments);
    scheme_element_free((scheme_element *)evaluatedList);
    return (scheme_element *)scheme_number_new(sum);
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_add()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_add, PROCEDURE_ADD_NAME, _add_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_add.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_add.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_add;
}
