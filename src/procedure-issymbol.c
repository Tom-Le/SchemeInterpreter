#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "scheme-pair-utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-issymbol.h"

/**** Private variables ****/

static scheme_procedure _procedure_issymbol;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "symbol?".
 *
 * Check if given argument is a symbol.
 *
 * Will return NULL if:
 * - Supplied element is not in the format (<element>).
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Scheme boolean #t if argument is a symbol, #f if not,
 *         or NULL if an error occurred.
 */
static scheme_element *_issymbol_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_issymbol_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
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
    int isSymbol = scheme_element_is_type(arg, scheme_symbol_get_type());
    scheme_element_free(arg);

    if (isSymbol)
        return (scheme_element *)scheme_boolean_get_true();
    else
        return (scheme_element *)scheme_boolean_get_false();
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_issymbol()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_issymbol, PROCEDURE_ISSYMBOL_NAME, _issymbol_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_issymbol.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_issymbol.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_issymbol;
}
