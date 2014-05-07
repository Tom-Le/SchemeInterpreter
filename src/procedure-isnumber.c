#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-isnumber.h"

/**** Private variables ****/

static scheme_procedure _procedure_isnumber;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "number?".
 *
 * Check if given argument is a number.
 *
 * Will return NULL if:
 * - Supplied element is not in the format (<element>).
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Scheme boolean #t if argument is a number, #f if not,
 *         or NULL if an error occurred.
 */
static scheme_element *_isnumber_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_isnumber_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
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
    arg = scheme_evaluate(arg, namespace);

    if (arg == NULL)
    {
        // Could not evaluate argument.
        return NULL;
    }

    // Check arg's type.
    int isNumber = scheme_element_is_type(arg, scheme_number_get_type());
    scheme_element_free(arg);

    if (isNumber)
        return (scheme_element *)scheme_boolean_get_true();
    else
        return (scheme_element *)scheme_boolean_get_false();
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_isnumber()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_isnumber, PROCEDURE_ISNUMBER_NAME, _isnumber_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_isnumber.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_isnumber.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_isnumber;
}

