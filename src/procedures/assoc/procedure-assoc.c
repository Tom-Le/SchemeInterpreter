#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "utils.h"
#include "scheme-procedure-init.h"

#include "procedure-assoc.h"

/**** Private variables ****/

static scheme_procedure _procedure_assoc;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "assoc".
 *
 * Find a pair among a list of pairs that is associated with a given element,
 * or #f if the given element does not associate with any pair in the list.
 *
 * Will return NULL if:
 * - Supplied element is not a list in the format: (<element> <list>).
 * - An element in <list> is not in the format (<element> ...).
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Associated pair as described, or #f if no pair is found, or NULL
 *         if an error occurs.
 */
static scheme_element *_assoc_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_assoc_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    // Get arguments.
    int argCount;
    scheme_element **args = scheme_list_to_array((scheme_pair *)element, &argCount);

    // Check if argument list is invalid.
    if (argCount == -1) return NULL;
    else if (argCount != 2)
    {
        // Wrong number of arguments.
        if (args != NULL) free(args);
        return NULL;
    }

    scheme_element *key = args[0];
    scheme_element *list = args[1];
    free(args);

    // Evaluate arguments.
    key = scheme_evaluate(key, namespace);
    list = scheme_evaluate(list, namespace);
    if (key == NULL || list == NULL)
    {
        scheme_element_free(key);
        scheme_element_free(list);
        return NULL;
    }

    // Get items in list.
    scheme_element **pairs = scheme_list_to_array((scheme_pair *)list, &argCount);

    scheme_element *ret = (scheme_element *)scheme_boolean_get_false();
    if (argCount == -1)
    {
        // List is not in the right format.
        ret = NULL;
    }
    else
    {
        // Go through each item in list.
        for (int i = 0; i < argCount; ++i)
        {
            scheme_element *p = *(pairs + i);

            // Each item in list must be a pair.
            if (!scheme_element_is_type(p, scheme_pair_get_type()))
            {
                ret = NULL;
                break;
            }

            scheme_element *first = scheme_pair_get_first((scheme_pair *)p);

            if (scheme_element_compare(key, first))
            {
                // Found a pair that is associated with key.
                ret = scheme_element_copy(p);
                break;
            }
        }
    }

    scheme_element_free(key);
    scheme_element_free(list);
    free(pairs);
    return ret;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_get()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_assoc, PROCEDURE_ASSOC_NAME, _assoc_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_assoc.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_assoc.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_assoc;
}
