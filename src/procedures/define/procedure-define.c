#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-define.h"

/**** Private variables ****/

static scheme_procedure _procedure_define;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementations of Scheme procedure "define".
 *
 * Associate an identifier with a Scheme element in current namespace.
 *
 * Will return NULL if:
 * - Supplied element is not a pair in the format: (<symbol> <element>).
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Element recently associated with an identifier or NULL if
 *         an error occurs.
 */
static scheme_element *_define_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_define_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    // Get arguments.
    int argCount;
    scheme_element **args = scheme_list_to_array((scheme_pair *)element, &argCount);

    // Check if argument list is invalid.
    if (argCount == -1) return NULL;
    else if (argCount < 2)
    {
        // Wrong number of arguments.
        if (args != NULL) free(args);
        return NULL;
    }

    scheme_element *first = args[0];

    if (scheme_element_is_type(first, scheme_symbol_get_type()))
    {
        // Usage 1: Associate an identifier with a Scheme element.
        // Must have exactly 2 arguments.
        if (argCount != 2)
        {
            free(args);
            return NULL;
        }

        scheme_symbol *symbol = (scheme_symbol *)first;
        scheme_element *symbolElement = args[1];
        free(args);

        // Evaluate element to be defined.
        symbolElement = scheme_evaluate(symbolElement, namespace);
        if (symbolElement == NULL) return NULL;

        // Store in namespace.
        char *symbolName = scheme_symbol_get_value(symbol);
        scheme_namespace_set(namespace, symbolName, symbolElement);
        free(symbolName);

        return symbolElement;
    }
    else if (scheme_element_is_type(first, scheme_pair_get_type()))
    {
        // Usage 2: Define a lambda function.
        free(args);

        // Element must not be the empty pair.
        if (scheme_pair_is_empty((scheme_pair *)first))
        {
            return NULL;
        }

        // Get procedure's name.
        scheme_element *nameSymbol = scheme_pair_get_first((scheme_pair *)first);
        if (!scheme_element_is_type(nameSymbol, scheme_symbol_get_type()))
        {
            return NULL;
        }
        char *name = scheme_symbol_get_value((scheme_symbol *)nameSymbol);

        // Get list of arguments.
        scheme_element *arguments = scheme_pair_get_second((scheme_pair *)first);
        // Get list of expressions: Second elemnet of input list.
        scheme_element *expressions = scheme_pair_get_second((scheme_pair *)element);

        scheme_lambda *result = scheme_lambda_new_from_elements(name, arguments, expressions);

        // Store procedure in namespace.
        scheme_namespace_set(namespace, name, (scheme_element *)result);
        free(name);

        return (scheme_element *)result;
    }

    // Invalid first element.
    return NULL;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_get()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_define, PROCEDURE_DEFINE_NAME, _define_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_define.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_define.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_define;
}
