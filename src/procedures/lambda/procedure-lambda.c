#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-lambda.h"

/**** Private variables ****/

static scheme_procedure _procedure_lambda;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**
 * Implementation of Scheme procedure "lambda"
 * Return an anonymous procedure.
 *
 * Will return NULL if:
 * - Supplied element is not a list in the format:
 *       ((<arguments> ... [. <rest argument>]) <expression> ...)
 *   or:
 *       (<rest argument> <expression> ...)
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Anonymous procedure, or NULL if an error occurs.
 */
static scheme_element *_lambda_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated procedure.
 * This function does nothing.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_lambda_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    // Element must be a non-empty pair.
    if (!scheme_element_is_type(element, scheme_pair_get_type()))
        return NULL;
    if (scheme_pair_is_empty((scheme_pair *)element))
        return NULL;

    // Get list of arguments.
    scheme_element *arguments = scheme_pair_get_first((scheme_pair *)element);

    // Get list of expressions.
    scheme_element *expressions = scheme_pair_get_second((scheme_pair *)element);

    return (scheme_element *)scheme_lambda_new_from_elements(NULL, arguments, expressions);
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_get()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_lambda, PROCEDURE_LAMBDA_NAME, _lambda_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_lambda.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_lambda.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_lambda;
}
