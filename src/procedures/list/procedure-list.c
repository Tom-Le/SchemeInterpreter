#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-list.h"

/**** Private variables ****/

static scheme_procedure _procedure_list;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "list".
 *
 * Will return NULL if:
 * - Element is not a list in the format: (<element> ...).
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Newly created list or NULL if an error occurs.
 */
static scheme_element *_list_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_list_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    // Evaluate each argument.
    scheme_pair *evaluatedList = scheme_list_evaluated((scheme_pair *)element, namespace);
    if (evaluatedList == NULL)
    {
        return NULL;
    }

    // Count number of elements in evaluated list and verify that it is indeed a list.
    int argCount;
    scheme_element **arguments = scheme_list_to_array(evaluatedList, &argCount);
    free(arguments);

    // Terminate if element is not a list.
    if (argCount == -1)
    {
        return NULL;
    }

    return (scheme_element *)evaluatedList;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_get()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_list, PROCEDURE_LIST_NAME, _list_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_list.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_list.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_list;
}

