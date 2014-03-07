#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "scheme-procedure-init.h"

#include "procedure-quote.h"

/**** Private variables ****/

static scheme_procedure _procedure_quote;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "quote".
 * Return the supplied argument as-is.
 *
 * Will return NULL if:
 *   - Supplied element is not in the format (<element>).
 *   - Out of memory.
 *
 * @param  element  A Scheme element.
 *
 * @return Supplied element as-is.
 */
static scheme_element *_quote_function(scheme_element *element);

/**** Private function implementations ****/

static scheme_element *_quote_function(scheme_element *element)
{
    // Element must be a pair.
    if (!scheme_element_is_type(element, SCHEME_PAIR_TYPE))
        return NULL;

    // Element must not be the empty pair.
    if (scheme_pair_is_empty((scheme_pair *)element))
        return NULL;

    // Make sure there is only one argument.
    scheme_element *rest = scheme_pair_get_second((scheme_pair *)element);
    if (!scheme_pair_is_empty((scheme_pair *)rest))
        return NULL;

    // Return copy of pair's first element.
    scheme_element *first = scheme_pair_get_first((scheme_pair *)element);
    return scheme_element_copy(first);
}

scheme_procedure *scheme_procedure_quote()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_quote, PROCEDURE_QUOTE_NAME, _quote_function);
        _proc_initd = 1;
    }

    return &_procedure_quote;
}
