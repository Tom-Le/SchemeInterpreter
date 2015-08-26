#include <stdlib.h>

#include "eval.h"
#include "utils.h"

#include "scheme-data-types.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-cadr.h"

static scheme_procedure _procedure_cadr;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "cadr".
 *
 * Will return NULL if:
 * - Supplied element is not a pair in the format: (<pair>)
 * - Argument <pair> is not a list with at least 2 elements.
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Result of evaluation or NULL if an error occurs.
 */
static scheme_element *_cadr_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_cadr_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    scheme_element *result, *parameter, *temp;

    // Get procedures car and cdr.
    scheme_procedure *procedure_car = (scheme_procedure *)scheme_namespace_get(namespace, "car");
    if (!scheme_element_is_type((scheme_element *)procedure_car, scheme_procedure_get_type()))
    {
        return NULL;
    }
    scheme_procedure *procedure_cdr = (scheme_procedure *)scheme_namespace_get(namespace, "cdr");
    if (!scheme_element_is_type((scheme_element *)procedure_cdr, scheme_procedure_get_type()))
    {
        return NULL;
    }

    // Call cdr on given pair.
    parameter = element;
    result = scheme_procedure_apply(procedure_cdr, parameter, namespace);
    if (result == NULL)
    {
        return NULL;
    }

    // Call car on resulting pair.
    // Quote result of cdr.
    parameter = (scheme_element *)scheme_element_quote(result);
    scheme_element_free(result);

    // Put quoted result in a list of 1 element.
    temp = (scheme_element *)scheme_pair_new(parameter, (scheme_element *)scheme_pair_get_empty());
    scheme_element_free(parameter);
    parameter = temp;

    // Perform car call.
    result = scheme_procedure_apply(procedure_car, parameter, namespace);
    scheme_element_free(parameter);

    return result;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_get()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_cadr, PROCEDURE_CADR_NAME, _cadr_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_cadr.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_cadr.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_cadr;
}
