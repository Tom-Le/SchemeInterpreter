#include <stdlib.h>

#include "eval.h"
#include "utils.h"

#include "scheme-data-types.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-caddddr.h"

static scheme_procedure _procedure_caddddr;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "caddddr".
 *
 * Will return NULL if:
 * - Supplied element is not a pair in the format: (<pair>)
 * - Argument <pair> is not a list with at least 5 elements.
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Result of evaluation or NULL if an error occurs.
 */
static scheme_element *_caddddr_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_caddddr_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
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

    // Call cdr 4 times.
    parameter = scheme_element_copy(element);
    for (int i = 0; i < 4; ++i)
    {
        // Perform cdr call.
        result = scheme_procedure_apply(procedure_cdr, parameter, namespace);
        scheme_element_free(parameter);

        if (result == NULL)
        {
            return NULL;
        }

        // Quote result.
        parameter = (scheme_element *)scheme_element_quote(result);
        scheme_element_free(result);

        // Add quoted result to a list of 1 element.
        temp = (scheme_element *)scheme_pair_new(parameter, (scheme_element *)scheme_pair_get_empty());
        scheme_element_free(parameter);
        parameter = temp;
    }

    // Call car on resulting list.
    result = scheme_procedure_apply(procedure_car, parameter, namespace);
    scheme_element_free(parameter);

    return result;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_get()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_caddddr, PROCEDURE_CADDDDR_NAME, _caddddr_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_caddddr.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_caddddr.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_caddddr;
}
