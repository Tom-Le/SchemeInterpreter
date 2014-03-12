#include <stdlib.h>

#include "eval.h"
#include "procedure-utils.h"
#include "scheme-data-types.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-cond.h"

/**** Private variables ****/

static scheme_procedure _procedure_cond;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "cond".
 *
 *    (cond (<condition> [<expression>]) ...)
 *
 * Return the result of the first <expression> whose <condition> either does not
 * evaluate to #f or is the symbol "else".
 *
 * If there is no <expression>, returns <condition> unless <condition> is the symbol
 * "else", in which case we return NULL.
 *
 * Block with "else" for condition must be the last block.
 *
 * Will return NULL if:
 * - Supplied element is not a pair in the format of: ((<condition> [<expression>]) ...).
 * - Any of the condition-expression block is not a list in the given format.
 * - All <condition> expressions evaluate to #f.
 * - There exists a block without an <expression> whose <condition> is "else".
 * - A block with condition "else" is not the last block.
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Result or NULL as described above.
 */
static scheme_element *_cond_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Process a condition-expression block.
 *
 * The block should be a list in the format:
 *
 *     (<condition> [<expression>] ...)
 *
 * Return NULL if condition could not be evaluated.
 *
 * If there is only a condition by itself, return #f if condition evaluates
 * to #f, NULL if condition is "else" or return evaluation of condition.
 *
 * If condition is followed by one or more expressions, return #f if condition
 * evaluates to #f. Otherwise, evaluate expressions sequentially and return result
 * of evaluating the last one. If any expression evaluates to NULL, return NULL
 *
 * @param  element    Condition-expression block.
 * @param  namespace  Active namespace.
 *
 * @return Result as described above.
 */
static scheme_element *_parse_condition_block(scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_cond_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    // Get arguments.
    int argCount;
    scheme_element **args = procedure_get_arguments(element, &argCount);

    // Check if argument list is valid and non-empty.
    if (argCount == -1 || argCount == 0)
        return NULL;

    // Verify that every block is a non-empty pair, and that "else" block only appears
    // at the very end.
    for (int i = 0; i < argCount - 1; ++i)
    {
        scheme_element *block = *(args + i);

        if (!scheme_element_is_type(block, SCHEME_PAIR_TYPE) || scheme_pair_is_empty((scheme_pair *)block))
        {
            free(args);
            return NULL;
        }

        scheme_element *condition = scheme_pair_get_first((scheme_pair *)block);

        if (i < argCount - 2
                && scheme_element_is_type(condition, SCHEME_SYMBOL_TYPE)
                && scheme_symbol_value_equals((scheme_symbol *)condition, "else"))
        {
            free(args);
            return NULL;
        }
    }

    // Go through each condition-expression block.
    scheme_element *ret = scheme_void_get();
    for (int i = 0; i < argCount; ++i)
    {
        scheme_element *block = *(args + i);
        scheme_element *result = _parse_condition_block(block, namespace);

        if (result == NULL)
        {
            // Could not evaluate.
            ret = NULL;
            break;
        }

        // Continue if condition was #f.
        if (result == (scheme_element *)scheme_boolean_get_false()) continue;

        // Return result if condition was not #f.
        ret = result;
        break;
    }

    free(args);
    return ret;
}

static scheme_element *_parse_condition_block(scheme_element *element, scheme_namespace *namespace)
{
    scheme_pair *block = (scheme_pair *)element;

    int exprCount;
    scheme_element **exprs = procedure_get_arguments((scheme_element *)block, &exprCount);

    // Check if condition-expression block is valid and non-empty.
    if (exprCount == -1 || exprCount == 0)
        return NULL;

    // Evaluate condition.
    scheme_element *condition = *exprs;
    int conditionIsElse = scheme_element_is_type(condition, SCHEME_SYMBOL_TYPE)
                       && scheme_symbol_value_equals((scheme_symbol *)condition, "else");

    condition = scheme_evaluate(condition, namespace);

    if (condition == NULL && !conditionIsElse)
    {
        // Could not evaluate condition.
        free(exprs);
        return NULL;
    }

    if (exprCount == 1)
    {
        // Block only has condition by itself.
        free(exprs); // No need for this.

        // Condition cannot be "else" symbol by itself.
        if (conditionIsElse)
        {
            scheme_element_free(condition);
            return NULL;
        }

        if (condition == (scheme_element *)scheme_boolean_get_false())
            return (scheme_element *)scheme_boolean_get_false();

        return condition;
    }
    else
    {
        // Block has at least one expression.
        int isFalse = condition == (scheme_element *)scheme_boolean_get_false();
        scheme_element_free(condition);

        // Only continue if condition does not evaluate to #f.
        if (isFalse)
        {
            free(exprs);
            return (scheme_element *)scheme_boolean_get_false();
        }

        // Evaluate each expression and return the result of the last one.
        scheme_element *expression;
        for (int e = 1; e < exprCount - 1; ++e)
        {
            expression = scheme_evaluate(*(exprs + e), namespace);

            if (expression == NULL)
            {
                free(exprs);
                return NULL;
            }

            scheme_element_free(expression);
        }

        expression = scheme_evaluate(*(exprs + exprCount - 1), namespace);

        free(exprs);
        return expression;
    }
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_cond()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_cond, PROCEDURE_COND_NAME, _cond_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_cond.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_cond.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_cond;
}
