#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-car.h"

/**** Private variables ****/

static scheme_procedure _procedure_car;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "car".
 *
 * Return the first element of the Scheme pair supplied as argument.
 *
 * Will return NULL if:
 * - Supplied element is not a pair in the format: (<pair>)
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return First element of pair, or NULL if an error occurs.
 */
static scheme_element *_car_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_car_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    // Get arguments.
    int argCount;
    scheme_element **args = scheme_list_to_array((scheme_pair *)element, &argCount);

    // Check if argument list is invalid.
    if (argCount == -1) return NULL;
    else if (argCount != 1)
    {
        // Wrong number of arguments.
        if (args != NULL) free(args);
        return NULL;
    }

    scheme_element *arg = *args;
    free(args);

    // Evaluate argument.
    scheme_element *result = scheme_evaluate(arg, namespace);
    if (result == NULL) return NULL;

    // Evaluated result must be a pair.
    if (!scheme_element_is_type(result, scheme_pair_get_type()))
    {
        scheme_element_free(result);
        return NULL;
    }

    // Get a copy of the first element of result.
    scheme_element *car = scheme_pair_get_first((scheme_pair *)result);
    car = scheme_element_copy(car);

    scheme_element_free(result);

    return car;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_car()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_car, PROCEDURE_CAR_NAME, _car_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_car.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_car.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_car;
}
