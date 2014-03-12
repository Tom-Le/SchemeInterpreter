#include <stdlib.h>

#include "eval.h"
#include "procedure-utils.h"
#include "scheme-data-types.h"
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
 * @return Element recently associated with an identifier
 *         or NULL if an error occurs.
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
    scheme_element **args = procedure_get_arguments(element, &argCount);

    // Check if argument list is invalid.
    if (argCount == -1) return NULL;
    else if (argCount != 2)
    {
        // Wrong number of arguments.
        if (args != NULL) free(args);
        return NULL;
    }

    scheme_element *symbol = args[0];
    scheme_element *symbolElement = args[1];
    free(args);

    // Halt if first argument is not a symbol.
    if (!scheme_element_is_type(symbol, SCHEME_SYMBOL_TYPE)) return NULL;

    // Evaluate element to be defined.
    symbolElement = scheme_evaluate(symbolElement, namespace);
    if (symbolElement == NULL) return NULL;

    char *symbolName = scheme_symbol_get_value((scheme_symbol *)symbol);
    scheme_namespace_set(namespace, symbolName, symbolElement);
    free(symbolName);

    return symbolElement;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_define()
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
