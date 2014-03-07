#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "scheme-procedure-init.h"

#include "procedure-cons.h"

/**** Private variables ****/

static scheme_procedure _procedure_cons;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "cons".
 * Return a Scheme pair with first and second elements set to the given
 * arguments.
 *
 * Will return NULL if:
 *   - Supplied element is not a pair in the format: (<element> <element>)
 *   - Out of memory.
 *
 * @param  element  A Scheme element.
 *
 * @return Constructed pair, or NULL if an error occurred.
 */
static scheme_element *_cons_function(scheme_element *element);

/**** Private function implementations ****/

static scheme_element *_cons_function(scheme_element *element)
{
    // Element must be a pair.
    if (!scheme_element_is_type(element, SCHEME_PAIR_TYPE))
        return NULL;

    // Element must not be the empty pair.
    if (scheme_pair_is_empty((scheme_pair *)element))
        return NULL;

    // Get first argument.
    scheme_element *firstArg = scheme_pair_get_first((scheme_pair *)element);

    // Get second argument.
    scheme_element *rest = scheme_pair_get_second((scheme_pair *)element);
    if (!scheme_element_is_type(rest, SCHEME_PAIR_TYPE))
        return NULL;
    scheme_element *secondArg = scheme_pair_get_first((scheme_pair *)rest);

    // Make sure there are no more arguments.
    rest = scheme_pair_get_second((scheme_pair *)rest);
    if (!scheme_pair_is_empty((scheme_pair *)rest))
        return NULL;

    // Evaluate first and second arguments.
    firstArg = scheme_evaluate(firstArg);
    secondArg = scheme_evaluate(secondArg);
    if (firstArg == NULL || secondArg == NULL)
    {
        scheme_element_free(firstArg);
        scheme_element_free(secondArg);
        return NULL;
    }

    return (scheme_element *)scheme_pair_new(firstArg, secondArg);
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_cons()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_cons, PROCEDURE_CONS_NAME, _cons_function);
        _proc_initd = 1;
    }

    return &_procedure_cons;
}
