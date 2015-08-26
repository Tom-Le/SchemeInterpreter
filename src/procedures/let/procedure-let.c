#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-let.h"

/**** Private variables ****/

static scheme_procedure _procedure_let;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**
 * Implementation of Scheme procedure "let"
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Anonymous procedure, or NULL if an error occurs.
 */
static scheme_element *_let_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated procedure.
 * This function does nothing.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_let_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    // Element must be a non-empty pair.
    if (!scheme_element_is_type(element, scheme_pair_get_type()))
        return NULL;
    if (scheme_pair_is_empty((scheme_pair *)element))
        return NULL;

    // Get arguments.
    int argCount;
    scheme_element **arguments = scheme_list_to_array((scheme_pair *)element, &argCount);
    // There must be at least 2 arguments.
    if (argCount < 2)
    {
        free(arguments);
        return NULL;
    }

    // Get first argument.
    scheme_element *first = arguments[0];
    free(arguments);

    scheme_element *lambdaData;
    char *procID = NULL;

    if (scheme_element_is_type(first, scheme_pair_get_type()))
    {
        // First form: Given a list of argument identifiers with values and a list of expressions.
        lambdaData = element;
    }
    else if (scheme_element_is_type(first, scheme_symbol_get_type()))
    {
        // Second form: Given procedure ID, then a list of argument identifiers with values and a list of expressions.
        lambdaData = scheme_pair_get_second((scheme_pair *)element);

        if (!scheme_element_is_type(lambdaData, scheme_pair_get_type()))
        {
            return NULL;
        }

        procID = scheme_symbol_get_value((scheme_symbol *)first);
    }
    else
    {
        return NULL;
    }

    // Get list of arguments (treat it as a list of arguments with default values).
    scheme_element *argList = scheme_pair_get_first((scheme_pair *)lambdaData);

    // Get list of expressions.
    scheme_element *expressions = scheme_pair_get_second((scheme_pair *)lambdaData);

    // Create procedure from given arguments and expressions.
    scheme_lambda *lambda = scheme_lambda_new_from_elements(procID, argList, expressions);
    if (lambda == NULL)
    {
        return NULL;
    }

    // If call is in second form, also bind given procedure ID to lambda procedure in a local namespace.
    scheme_namespace *localNamespace = namespace;
    if (scheme_element_is_type(first, scheme_symbol_get_type()))
    {
        localNamespace = scheme_namespace_new(namespace);

        scheme_namespace_set(localNamespace, procID, (scheme_element *)lambda);
        free(procID);
    }

    // Run procedure with no arguments and return result.
    scheme_element *result = scheme_procedure_apply((scheme_procedure *)lambda, (scheme_element *)scheme_pair_get_empty(), localNamespace);

    scheme_element_free((scheme_element *)lambda);
    if (localNamespace != namespace)
    {
        scheme_element_free((scheme_element *)localNamespace);
    }

    return result;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_get()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_let, PROCEDURE_LET_NAME, _let_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_let.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_let.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_let;
}

