#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "scheme-procedure-init.h"

#include "procedure-symbol.h"

/**** Private variables ****/

static scheme_procedure _procedure_symbol;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "symbol?".
 * Check if given argument is a symbol.
 *
 * Will return NULL if:
 *   - Supplied element is not in the format (<element>).
 *   - Out of memory.
 *
 * @param  element  A Scheme element.
 *
 * @return Scheme boolean #t if argument is a symbol, #f if not,
 *   or NULL if an error occurred.
 */
static scheme_element *_symbol_function(scheme_element *element);

/**** Private function implementations ****/

static scheme_element *_symbol_function(scheme_element *element)
{
    // Element must be a pair.
    if (!scheme_element_is_type(element, SCHEME_PAIR_TYPE))
        return NULL;

    // Element must not be the empty pair.
    if (scheme_pair_is_empty((scheme_pair *)element))
        return NULL;

    // Get argument.
    scheme_element *arg = scheme_pair_get_first((scheme_pair *)element);

    // Make sure there are no more arguments.
    scheme_element *rest = scheme_pair_get_second((scheme_pair *)element);
    if (!scheme_pair_is_empty((scheme_pair *)rest))
        return NULL;

    // Evaluate argument.
    arg = scheme_evaluate(arg);

    // Check arg's type.
    int isSymbol = scheme_element_is_type(arg, SCHEME_SYMBOL_TYPE);
    scheme_element_free(arg);

    if (isSymbol)
        return (scheme_element *)scheme_boolean_get_true();
    else
        return (scheme_element *)scheme_boolean_get_false();
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_symbol()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_symbol, PROCEDURE_SYMBOL_NAME, _symbol_function);
        _proc_initd = 1;
    }

    return &_procedure_symbol;
}
