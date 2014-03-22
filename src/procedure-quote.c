#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "scheme-pair-utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-quote.h"

/**** Private variables ****/

static scheme_procedure _procedure_quote;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "quote".
 *
 * Return the given argument as-is.
 *
 * Will return NULL if:
 * - Supplied element is not in the format (<element>).
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Supplied element as-is.
 */
static scheme_element *_quote_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_quote_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
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

    // Simply return a copy of argument.
    return scheme_element_copy(arg);
}

scheme_procedure *scheme_procedure_quote()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_quote, PROCEDURE_QUOTE_NAME, _quote_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_quote.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_quote.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_quote;
}
