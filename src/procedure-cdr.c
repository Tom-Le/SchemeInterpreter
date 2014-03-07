#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "scheme-procedure-init.h"

#include "procedure-cdr.h"

/**** Private variables ****/

static scheme_procedure _procedure_cdr;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "cdr".
 * Return the second element of the Scheme pair supplied as argument.
 *
 * Will return NULL if:
 *   - Supplied element is not a pair in the format: (<pair>)
 *   - Out of memory.
 *
 * @param  element  A Scheme element.
 *
 * @return Second element, or NULL if an error occurred.
 */
static scheme_element *_cdr_function(scheme_element *element);

/**** Private function implementations ****/

static scheme_element *_cdr_function(scheme_element *element)
{
    // Element must be a pair.
    if (!scheme_element_is_type(element, SCHEME_PAIR_TYPE))
        return NULL;

    // Get argument.
    scheme_element *arg = scheme_pair_get_first((scheme_pair *)element);
    if (arg == NULL) return NULL;

    // Make sure there are no more arguments.
    scheme_element *rest = scheme_pair_get_second((scheme_pair *)element);
    if (!scheme_pair_is_empty((scheme_pair *)rest))
        return NULL;

    // Evaluate argument.
    scheme_element *result = scheme_evaluate(arg);

    // Evaluated result must be a pair.
    if (!scheme_element_is_type(result, SCHEME_PAIR_TYPE))
    {
        scheme_element_free(result);
        return NULL;
    }

    // Get a copy of the second element of result.
    scheme_element *cdr = scheme_pair_get_second((scheme_pair *)result);
    cdr = scheme_element_copy(cdr);

    scheme_element_free(result);

    return cdr;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_cdr()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_cdr, PROCEDURE_CDR_NAME, _cdr_function);
        _proc_initd = 1;
    }

    return &_procedure_cdr;
}
