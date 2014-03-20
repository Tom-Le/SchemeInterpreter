#include <stdlib.h>

#include "eval.h"
#include "scheme-pair-utils.h"

int scheme_pair_is_list(scheme_pair *list)
{
    while (1)
    {
        // Element must be a pair.
        if (!scheme_element_is_type((scheme_element *)list, scheme_pair_get_type()))
            return 0;

        // If we have reached the empty pair, then element is a list.
        if (scheme_pair_is_empty(list))
            return 1;

        list = (scheme_pair *)scheme_pair_get_second(list);
    }
}

scheme_element **scheme_list_to_array(scheme_pair *list, int *count)
{
    // Element must be a pair.
    if (!scheme_element_is_type((scheme_element *)list, scheme_pair_get_type()))
    {
        if (count != NULL) *count = -1;
        return NULL;
    }

    // Check if it is the empty pair.
    if (scheme_pair_is_empty(list))
    {
        if (count != NULL) *count = 0;
        return NULL;
    }

    // Count number of arguments.
    int argCount = 0;
    scheme_element *rest = (scheme_element *)list;
    while (!scheme_pair_is_empty((scheme_pair *)rest))
    {
        ++argCount;

        rest = scheme_pair_get_second((scheme_pair *)rest);
        if (!scheme_element_is_type(rest, scheme_pair_get_type()))
        {
            // Not a list.
            if (count != NULL) *count = -1;
            return NULL;
        }
    }

    scheme_element **array = (scheme_element **)malloc(sizeof(scheme_element *) * argCount);
    if (list == NULL)
    {
        *count = -1;
        return NULL;
    }

    scheme_pair *listRest = list;
    for (int i = 0; i < argCount; ++i)
    {
        // In the process of counting number of arguments above, we have already verified
        // that 'listRest' is a non-empty pair.
        array[i] = scheme_pair_get_first(listRest);
        listRest = (scheme_pair *)scheme_pair_get_second(listRest);
    }

    if (count != NULL) *count = argCount;
    return array;
}

scheme_pair *scheme_list_evaluated(scheme_pair *list, scheme_namespace *namespace)
{
    if (!scheme_element_is_type((scheme_element *)list, scheme_pair_get_type()))
    {
        return NULL;
    }

    if (scheme_pair_is_empty(list))
    {
        return list;
    }

    // Evaluate first element.
    scheme_element *evaluatedFirst = scheme_pair_get_first(list);
    evaluatedFirst = scheme_evaluate(evaluatedFirst, namespace);
    if (evaluatedFirst == NULL) return NULL;

    // Evaluate second element.
    scheme_element *evaluatedSecond = scheme_pair_get_second(list);
    if (scheme_element_is_type(evaluatedSecond, scheme_pair_get_type()))
    {
        // Second element is a list. Evaluate list recursively.
        evaluatedSecond = (scheme_element *)scheme_list_evaluated((scheme_pair *)evaluatedSecond, namespace);
    }
    else
    {
        // Simply evaluate second element.
        evaluatedSecond = scheme_evaluate(evaluatedSecond, namespace);
    }

    if (evaluatedSecond == NULL)
    {
        // Could not evaluate second element.
        scheme_element_free(evaluatedFirst);
        return NULL;
    }

    scheme_pair *result = scheme_pair_new(evaluatedFirst, evaluatedSecond);

    scheme_element_free(evaluatedFirst);
    scheme_element_free(evaluatedSecond);

    return result;
}
