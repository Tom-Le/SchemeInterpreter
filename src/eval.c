#include <stdlib.h>
#include <string.h>

#include "eval.h"

/**** Public function implementations ****/

scheme_element *scheme_evaluate(scheme_element *element, scheme_namespace *namespace)
{
    if (element == NULL) return NULL;

    // If element is a symbol, resolve it.
    if (scheme_element_is_type(element, SCHEME_SYMBOL_TYPE))
    {
        char *symbolValue = scheme_symbol_get_value((scheme_symbol *)element);
        scheme_element *resolvedElement = scheme_namespace_get(namespace, symbolValue);
        free(symbolValue);

        return resolvedElement;
    }

    // If element is not a pair, simply return it.
    if (!scheme_element_is_type(element, SCHEME_PAIR_TYPE))
        return scheme_element_copy(element);

    // Element is a pair.
    // Evaluate first element.
    scheme_pair *pair = (scheme_pair *)element;
    scheme_element *first = scheme_pair_get_first(pair);;
    first = scheme_evaluate(first, namespace);

    // Evaluated first element must be a procedure.
    if (!scheme_element_is_type(first, SCHEME_PROCEDURE_TYPE))
    {
        scheme_element_free(first);

        return NULL;
    }

    // Use procedure to evaluate second element of pair and return result.
    scheme_element *second = scheme_pair_get_second(pair);
    scheme_element *result = scheme_procedure_apply((scheme_procedure *)first, second, namespace);
    scheme_element_free(first);
    return result;
}
