#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "scheme-pair-utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-if.h"

/**** Private variables ****/

static scheme_procedure _procedure_if;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "if".
 *
 *     (if <test-expr> <then-expr> <else-expr>)
 *
 * Takes a test expression and two additional expressions. If test expression
 * evaluates to anything other than #f, first out of the two additional
 * expressions is evaluated and returned. Otherwise, second expression is
 * evaluated and returned.
 *
 * Will return NULL if:
 * - Supplied element is not a list with exactly three arguments as described above.
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Result or NULL as described above.
 */
static scheme_element *_if_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementation ****/

static scheme_element *_if_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    // Evaluate each argument.
    scheme_pair *evaluatedList = scheme_list_evaluated((scheme_pair *)element, namespace);
    if (evaluatedList == NULL)
    {
        return NULL;
    }

    // Get arguments.
    int argCount;
    scheme_element **args = scheme_list_to_array(evaluatedList, &argCount);

    // Check if argument list is valid and that we are given exactly 3 arguments.
    if (argCount == -1)
    {
        scheme_element_free((scheme_element *)evaluatedList);
        return NULL;
    }
    else if (argCount != 3)
    {
        free(args);
        scheme_element_free((scheme_element *)evaluatedList);
        return NULL;
    }

    scheme_element *condition = args[0];
    scheme_element *thenExpr = args[1];
    scheme_element *elseExpr = args[2];
    free(args);

    // Determine result based on condition.
    scheme_element *result;
    if (scheme_element_compare(condition, (scheme_element *)scheme_boolean_get_false()))
    {
        result = scheme_element_copy(elseExpr);
    }
    else
    {
        result = scheme_element_copy(thenExpr);
    }

    scheme_element_free((scheme_element *)evaluatedList);
    return result;
}

/**** Public function implementation ****/

scheme_procedure *scheme_procedure_if()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_if, PROCEDURE_IF_NAME, _if_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_if.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_if.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_if;
}
