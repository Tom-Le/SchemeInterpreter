#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "scheme-pair-utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-append.h"

/**** Private variables ***/

static scheme_procedure _procedure_append;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "append".
 * Return a new list with the given element appended onto the end of the
 * given list.
 *
 * Will return NULL if:
 * - Supplied element is not a pair in the format: (<list> <element>).
 * - First argument is not a list, defined as either the empty pair or
 *   a pair whose second element is also a list.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return New list as described, or NULL if an error occurs.
 */
static scheme_element *_append_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Create a copy of given list with element appended to the end.
 *
 * If list is empty, simply return copy of element.
 * If list does not end with an empty pair (ie. if it is not a list at all),
 * return NULL.
 *
 * @param  list     A list.
 * @param  element  A Scheme element.
 *
 * @return As described.
 */
static scheme_element *_append_list(scheme_pair *list, scheme_element *element);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_append_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
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

    scheme_element *firstArg = *args;
    scheme_element *secondArg = *(args + 1);
    free(args);

    // Evaluate arguments.
    firstArg = scheme_evaluate(firstArg, namespace);
    secondArg = scheme_evaluate(secondArg, namespace);
    if (firstArg == NULL || secondArg == NULL)
    {
        scheme_element_free(firstArg);
        scheme_element_free(secondArg);
        return NULL;
    }

    // First argument must resolve to a pair and second argument must
    // not resolve to NULL
    if (!scheme_element_is_type(firstArg, scheme_pair_get_type()) || secondArg == NULL)
    {
        scheme_element_free(firstArg);
        scheme_element_free(secondArg);
        return NULL;
    }

    scheme_element *newList = _append_list((scheme_pair *)firstArg, secondArg);
    scheme_element_free(firstArg);
    scheme_element_free(secondArg);
    return newList;
}

static scheme_element *_append_list(scheme_pair *list, scheme_element *element)
{
    // If list is empty, simply return copy of element.
    if (scheme_pair_is_empty(list))
        return scheme_element_copy(element);

    // Second element of list must be a pair.
    scheme_element *second = scheme_pair_get_second(list);
    if (!scheme_element_is_type(second, scheme_pair_get_type()))
        return NULL;

    // Append element onto second element of list.
    scheme_element *secondAppend = _append_list((scheme_pair *)second, element);
    if (secondAppend == NULL) return NULL;

    // Create new pair.
    scheme_element *first = scheme_pair_get_first(list);
    scheme_pair *pair = scheme_pair_new(first, secondAppend);

    scheme_element_free(secondAppend);
    return (scheme_element *)pair;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_append()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_append, PROCEDURE_APPEND_NAME, _append_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_append.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_append.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_append;
}
