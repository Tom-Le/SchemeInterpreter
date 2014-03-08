#include <stdlib.h>

#include "procedure-utils.h"

/**** Public function implementations ****/

scheme_element **procedure_get_arguments(scheme_element *element, int *count)
{
    // Element must be a pair.
    if (!scheme_element_is_type(element, SCHEME_PAIR_TYPE))
    {
        if (count != NULL) *count = -1;
        return NULL;
    }

    // Check if element is the empty pair.
    if (scheme_pair_is_empty((scheme_pair *)element))
    {
        if (count != NULL) *count = 0;
        return NULL;
    }

    // Count number of arguments.
    int argCount = 0;
    scheme_element *rest = element;
    while (!scheme_pair_is_empty((scheme_pair *)rest))
    {
        ++argCount;

        rest = scheme_pair_get_second((scheme_pair *)rest);
        if (!scheme_element_is_type(rest, SCHEME_PAIR_TYPE))
        {
            // Invalid list of argument.
            if (count != NULL) *count = -1;
            return NULL;
        }
    }

    // Allocate list of arguments.
    scheme_element **list = (scheme_element **)malloc(sizeof(scheme_element *) * argCount);
    rest = element;
    for (int i = 0; i < argCount; ++i)
    {
        // In the process of counting number of arguments above, we have already verified
        // that 'rest' is a non-empty pair.
        list[i] = scheme_pair_get_first((scheme_pair *)rest);
        rest = scheme_pair_get_second((scheme_pair *)rest);
    }

    if (count != NULL) *count = argCount;
    return list;
}
