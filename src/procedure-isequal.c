#include <stdlib.h>
#include <string.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "utils.h"
#include "scheme-procedure-init.h"

#include "procedure-isequal.h"

/**** Private variables ****/

static scheme_procedure _procedure_equal;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "equal?".
 *
 * Compare two Scheme elements.
 *
 * Will return NULL if:
 * - Supplied element is not a pair in the format (<element> <element>).
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Scheme boolean #t if two elements are equal, #f if not, or NULL
 *         if an error occurs.
 */
static scheme_element *_equal_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_equal_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    // Evaluate each argument.
    scheme_pair *evaluatedList = scheme_list_evaluated((scheme_pair *)element, namespace);
    if (evaluatedList == NULL)
    {
        return NULL;
    }

    // Get arguments.
    int argCount;
    scheme_element **args = scheme_list_to_array(evaluatedList, &argCount);

    // Check if argument list is invalid.
    if (argCount == -1)
    {
        scheme_element_free((scheme_element *)evaluatedList);
        return NULL;
    }
    else if (argCount != 2)
    {
        // Wrong number of arguments.
        free(args);
        scheme_element_free((scheme_element *)evaluatedList);
        return NULL;
    }

    scheme_element *firstArg = args[0];
    scheme_element *secondArg = args[1];
    free(args);

    int comparison = scheme_element_compare(firstArg, secondArg);
    scheme_element_free((scheme_element *)evaluatedList);

    return (scheme_element *)(comparison ? scheme_boolean_get_true() : scheme_boolean_get_false());
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_isequal()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_equal, PROCEDURE_EQUAL_NAME, _equal_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_equal.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_equal.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_equal;
}
