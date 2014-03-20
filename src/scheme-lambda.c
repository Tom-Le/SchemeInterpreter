#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eval.h"
#include "scheme-lambda.h"
#include "scheme-pair-utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

// Struct for Scheme lambda procedure.
struct scheme_lambda {
    struct scheme_procedure super;
    struct scheme_lambda_argument *arguments;
    scheme_element **expressions;
    char *restID;
    int argumentCount;
    int expressionCount;
};

/**** Private function declarations ****/

/**
 * Free a lambda procedure.
 *
 * @param  element  Should be a lambda procedure.
 */
static void _vtable_free(scheme_element *element);

/**
 * Copy a lambda procedure.
 *
 * @param  element  Should be a lambda procedure.
 *
 * @return Copy or NULL if out of memory.
 */
static scheme_element *_vtable_copy(scheme_element *element);

/**
 * Print lambda procedure to stdout.
 *
 * @param  element  Should be a lambda procedure.
 */
static void _vtable_print(scheme_element *element);

/**
 * Compare a lambda procedure to another procedure.
 * Will return 0 if either pointer is not a lambda procedure.
 *
 * @param  element  Should be a lambda procedure.
 * @param  other    A Scheme element.
 *
 * @return 1 if equal, 0 otherwise.
 */
static int _vtable_compare(scheme_element *element, scheme_element *other);

/**
 * Evaluate expressions stored in lambda procedure with given arguments.
 *
 * This function is called when scheme_procedure_apply() receives a lambda
 * procedure as its first argument.
 *
 * Will return NULL if out of memory.
 *
 * @param  procedure  A lambda procedure.
 * @param  element    Scheme element that is the argument of the procedure
 *                    call.
 * @param  namespace  Active namespace.
 *
 * @return Result of last expression stored in lambda procedure after
 *         evaluating every expression, or NULL if an error occurs.
 */
static scheme_element *_lambda_function(scheme_procedure *procedure,
                                        scheme_element *element,
                                        scheme_namespace *namespace);

/**** Private variables ****/

// Global virtual function table.
static struct scheme_element_vtable _scheme_lambda_vtable = {
    .get_type = scheme_lambda_get_type,
    .free = _vtable_free,
    .print = _vtable_print,
    .copy = _vtable_copy,
    .compare = _vtable_compare
};

// scheme_procedure virtual function table.
static struct scheme_element_vtable _scheme_procedure_vtable;
static int _scheme_procedure_vtable_initialized = 0;

// Static struct for Scheme lambda procedure's type.
static struct scheme_element_type _scheme_lambda_type = {
    .super = NULL,
    .name = "scheme_lambda"
};
static int _scheme_lambda_type_initd = 0;

/**** Private function implementations ****/

static void _vtable_free(scheme_element *element)
{
    scheme_lambda *procedure = (scheme_lambda *)element;

    // Free argument IDs.
    if (procedure->arguments != NULL)
    {
        for (int i = 0; i < procedure->argumentCount; ++i)
        {
            free(procedure->arguments[i].id);

            if (procedure->arguments[i].defaultValue != NULL)
            {
                scheme_element_free(procedure->arguments[i].defaultValue);
            }
        }
        free(procedure->arguments);
    }

    if (procedure->restID != NULL)
        free(procedure->restID);

    // Free expressions.
    for (int i = 0; i < procedure->expressionCount; ++i)
    {
        scheme_element_free(procedure->expressions[i]);
    }
    free(procedure->expressions);

    _scheme_procedure_vtable.free(element);
}

static scheme_element *_vtable_copy(scheme_element *element)
{
    scheme_lambda *procedure = (scheme_lambda *)element;

    return (scheme_element *)scheme_lambda_new(procedure->super.name,
                                               procedure->arguments,
                                               procedure->argumentCount,
                                               procedure->restID,
                                               procedure->expressions,
                                               procedure->expressionCount);
}

static void _vtable_print(scheme_element *element)
{
    _scheme_procedure_vtable.print(element);
}

static int _vtable_compare(scheme_element *element, scheme_element *other)
{
    if (!scheme_element_is_type(other, &_scheme_lambda_type)) return 0;

    scheme_lambda *this = (scheme_lambda *)element;
    scheme_lambda *that = (scheme_lambda *)other;

    // Use scheme_procedure's comparison.
    if (!_scheme_procedure_vtable.compare(element, other)) return 0;

    // Compare rest IDs.
    if (this->restID != NULL || that->restID != NULL)
    {
        if (this->restID == NULL && that->restID != NULL) return 0;
        if (this->restID != NULL && that->restID == NULL) return 0;

        if (strcmp(this->restID, that->restID) != 0) return 0;
    }

    // Compare list of arguments.
    if (this->arguments != NULL || that->arguments != NULL)
    {
        if (this->arguments != NULL && that->arguments == NULL) return 0;
        if (this->arguments == NULL && that->arguments != NULL) return 0;

        if (this->argumentCount != that->argumentCount) return 0;

        for (int i = 0; i < this->argumentCount; ++i)
        {
            if (strcmp(this->arguments[i].id, that->arguments[i].id) != 0) return 0;

            if (this->arguments[i].defaultValue == NULL && that->arguments[i].defaultValue != NULL)
                return 0;
            if (this->arguments[i].defaultValue != NULL && that->arguments[i].defaultValue == NULL)
                return 0;
            if (!scheme_element_compare(this->arguments[i].defaultValue, that->arguments[i].defaultValue))
                return 0;
        }
    }

    // Compare list of expressions.
    if (this->expressionCount != that->expressionCount) return 0;
    for (int i = 0; i < this->expressionCount; ++i)
    {
        if (!scheme_element_compare(this->expressions[i], that->expressions[i])) return 0;
    }

    return 1;
}

static scheme_element *_lambda_function(scheme_procedure *procedure,
                                        scheme_element *element,
                                        scheme_namespace *namespace)
{
    scheme_lambda *lambda = (scheme_lambda *)procedure;
    scheme_element *argumentList = element;

    // Verify that we are given enough arguments.
    if (lambda->argumentCount == 0 && lambda->restID == NULL)
    {
        // Lambda procedure expects no argument.
        if (!scheme_element_is_type(argumentList, scheme_pair_get_type()) || !scheme_pair_is_empty((scheme_pair *)argumentList))
        {
            return NULL;
        }
    }
    else
    {
        int argIndex;
        for (argIndex = 0; argIndex < lambda->argumentCount; ++argIndex)
        {
            // No more argument needed as soon as we find one with a default value.
            if (lambda->arguments[argIndex].defaultValue != NULL)
                break;

            // Stop if argument list is not a proper list.
            if (!scheme_element_is_type(argumentList, scheme_pair_get_type()))
                return NULL;

            argumentList = scheme_pair_get_second((scheme_pair *)argumentList);
        }

        if (lambda->restID == NULL)
        {
            // Also verify that we do not have more arguments than is needed.
            for (int i = argIndex; i < lambda->argumentCount; ++i)
            {
                // Stop if argument list is not a pair (signifying that it was invalidly formatted).
                if (!scheme_element_is_type(argumentList, scheme_pair_get_type()))
                    return NULL;

                // Break if argument list is empty.
                if (scheme_pair_is_empty((scheme_pair *)argumentList))
                    break;

                argumentList = scheme_pair_get_second((scheme_pair *)argumentList);
            }

            // After the loop, argument list should have been fully exhausted.
            if (!scheme_element_is_type(argumentList, scheme_pair_get_type()))
                return NULL;
            if (!scheme_pair_is_empty((scheme_pair *)argumentList))
                return NULL;
        }
    }

    // Verify that the rest of the argument list is actually a list.
    if (!scheme_pair_is_list((scheme_pair *)argumentList))
        return NULL;

    // Set up local namespace.
    scheme_namespace *localNamespace = scheme_namespace_new(namespace);

    // Populate local namespace with arguments.
    // Enumerate through arguments and add them to namespace.
    argumentList = element;
    for (int i = 0; i < lambda->argumentCount; ++i)
    {
        char *id = lambda->arguments[i].id;

        scheme_element *argument;
        if (!scheme_pair_is_empty((scheme_pair *)argumentList))
        {
            argument = scheme_pair_get_first((scheme_pair *)argumentList);

            argument = scheme_evaluate(argument, namespace);
            if (argument == NULL)
            {
                scheme_element_free((scheme_element *)localNamespace);
                return NULL;
            }

            argumentList = scheme_pair_get_second((scheme_pair *)argumentList);
        }
        else
        {
            argument = scheme_element_copy(lambda->arguments[i].defaultValue);
        }

        scheme_namespace_set(localNamespace, id, argument);
        scheme_element_free(argument);
    }

    // Add the rest under rest argument if one exists.
    if (lambda->restID != NULL)
    {
        // Construct list with every element already evaluated.
        scheme_pair *evaluatedArgumentList = scheme_list_evaluated((scheme_pair *)argumentList, namespace);
        if (evaluatedArgumentList == NULL)
        {
            scheme_element_free((scheme_element *)localNamespace);
            return NULL;
        }

        scheme_namespace_set(localNamespace, lambda->restID, (scheme_element *)evaluatedArgumentList);

        scheme_element_free((scheme_element *)evaluatedArgumentList);
    }

    // Evaluate expressions.
    scheme_element *result = NULL;
    for (int i = 0; i < lambda->expressionCount; ++i)
    {
        scheme_element_free(result);
        result = scheme_evaluate(lambda->expressions[i], localNamespace);
    }

    scheme_element_free((scheme_element *)localNamespace);

    return result;
}

/**** Public function implementations ****/

scheme_lambda *scheme_lambda_new(char *name,
                                 struct scheme_lambda_argument *arguments,
                                 int argumentCount,
                                 char *restID,
                                 scheme_element **expressions,
                                 int expressionCount)
{
    // Verify argument ID array is valid.
    if (arguments != NULL)
    {
        int containsDefault = 0;
        for (int i = 0; i < argumentCount; ++i)
        {
            // Argument ID cannot be blank.
            if (arguments[i].id == NULL || strlen(arguments[i].id) == 0)
                return NULL;

            // There cannot be an argument without a default value after one with
            // a default value has been listed.
            if (arguments[i].defaultValue == NULL && containsDefault == 1)
                return NULL;
            else if (arguments[i].defaultValue != NULL)
                containsDefault = 1;
        }
    }

    if (expressions == NULL || expressionCount == 0)
        return NULL;

    scheme_lambda *procedure = malloc(sizeof(scheme_lambda));
    if (procedure == NULL) return NULL;

    // Call scheme_procedure's initializer.
    scheme_procedure_init((scheme_procedure *)procedure, name, _lambda_function);

    // Copy scheme_procedure's virtual function table.
    if (!_scheme_procedure_vtable_initialized)
    {
        scheme_element_vtable_clone(&_scheme_procedure_vtable, ((scheme_element *)procedure)->vtable);

        _scheme_procedure_vtable_initialized = 1;
    }

    // Set up our own virtual function table.
    ((scheme_element *)procedure)->vtable = &_scheme_lambda_vtable;

    // Copy argument IDs.
    if (arguments != NULL)
    {
        procedure->arguments = malloc(sizeof(struct scheme_lambda_argument) * argumentCount);
        if (procedure->arguments == NULL) return NULL;

        for (int i = 0; i < argumentCount; ++i)
        {
            int lengthID = strlen(arguments[i].id) + 1;
            if ((procedure->arguments[i].id = malloc(sizeof(char) * lengthID)) == NULL) return NULL;
            strcpy(procedure->arguments[i].id, arguments[i].id);

            if (arguments[i].defaultValue != NULL)
                procedure->arguments[i].defaultValue = scheme_element_copy(arguments[i].defaultValue);
            else
                procedure->arguments[i].defaultValue = NULL;
        }

        procedure->argumentCount = argumentCount;
    }
    else
    {
        procedure->arguments = NULL;
        procedure->argumentCount = 0;
    }

    // Copy rest ID.
    if (restID != NULL)
    {
        int lengthID = strlen(restID) + 1;
        if ((procedure->restID = malloc(sizeof(char) * lengthID)) == NULL) return NULL;
        strcpy(procedure->restID, restID);
    }
    else
        procedure->restID = NULL;

    // Copy expressions.
    procedure->expressions = malloc(sizeof(scheme_element *) * expressionCount);
    if (procedure->expressions == NULL) return NULL;

    procedure->expressionCount = expressionCount;
    for (int i = 0; i < expressionCount; ++i)
    {
        procedure->expressions[i] = scheme_element_copy(expressions[i]);
        if (procedure->expressions[i] == NULL) return NULL;
    }

    return procedure;
}

scheme_lambda *scheme_lambda_new_from_elements(char *name,
                                               scheme_element *arguments,
                                               scheme_element *expressions)
{
    struct scheme_lambda_argument *arguments_arr;
    int argumentCount;
    char *restID;
    scheme_element **expressions_arr;
    int expressionCount;

    // Arguments could be a symbol or a pair.
    if ( !(scheme_element_is_type(arguments, scheme_symbol_get_type()) || scheme_element_is_type(arguments, scheme_pair_get_type())) )
        return NULL;

    // Expressions must be a list.
    if ( !(scheme_element_is_type(expressions, scheme_pair_get_type()) && scheme_pair_is_list((scheme_pair *)expressions)) )
        return NULL;

    if (scheme_element_is_type(arguments, scheme_symbol_get_type()))
    {
        // If arguments is a symbol, procedure only has a rest ID.
        restID = scheme_symbol_get_value((scheme_symbol *)arguments);
        if (restID == NULL) return NULL;

        arguments_arr = NULL;
        argumentCount = 0;
    }
    else
    {
        // If argument is a pair, count number of argument IDs and verify argument list.
        scheme_element *argumentList = arguments;
        argumentCount = 0;
        while (1)
        {
            // Break if argument list is not a pair or the empty pair.
            if (!scheme_element_is_type(argumentList, scheme_pair_get_type()))
                break;
            if (scheme_pair_is_empty((scheme_pair *)argumentList))
                break;

            scheme_element *argument = scheme_pair_get_first((scheme_pair *)argumentList);

            // Argument in list must be a symbol or a pair.
            if (!(scheme_element_is_type(argument, scheme_symbol_get_type()) || scheme_element_is_type(argument, scheme_pair_get_type())))
                return NULL;

            if (scheme_element_is_type(argument, scheme_pair_get_type()))
            {
                // If argument is a pair (ie. there is a default value), it must be a list of exactly 2 elements.
                int count;
                scheme_element **argument_arr = scheme_list_to_array((scheme_pair *)argument, &count);

                if (count != 2)
                {
                    if (argument_arr != NULL) free(argument_arr);
                    return NULL;
                }

                // First element must be a symbol.
                scheme_element *first = argument_arr[0];
                free(argument_arr);
                if (!scheme_element_is_type(first, scheme_symbol_get_type()))
                {
                    return NULL;
                }
            }

            ++argumentCount;

            argumentList = scheme_pair_get_second((scheme_pair *)argumentList);
        }

        // Remainder of argument list must be a pair (ie. the empty pair) or a symbol.
        if ( !(   scheme_element_is_type(argumentList, scheme_pair_get_type())
               || scheme_element_is_type(argumentList, scheme_symbol_get_type())) )
            return NULL;

        if (argumentCount > 0)
        {
            // Create and populate arguments array.
            arguments_arr = malloc(sizeof(struct scheme_lambda_argument) * argumentCount);
            if (arguments_arr == NULL) return NULL;

            argumentList = arguments;
            for (int i = 0; i < argumentCount; ++i)
            {
                scheme_element *argument = scheme_pair_get_first((scheme_pair *)argumentList);

                if (scheme_element_is_type(argument, scheme_symbol_get_type()))
                {
                    arguments_arr[i].id = scheme_symbol_get_value((scheme_symbol *)argument);
                    arguments_arr[i].defaultValue = NULL;
                }
                else if (scheme_element_is_type(argument, scheme_pair_get_type()))
                {
                    scheme_element **argument_arr = scheme_list_to_array((scheme_pair *)argument, NULL);
                    arguments_arr[i].id = scheme_symbol_get_value((scheme_symbol *)argument_arr[0]);
                    arguments_arr[i].defaultValue = argument_arr[1];
                    free(argument_arr);
                }

                argumentList = scheme_pair_get_second((scheme_pair *)argumentList);
            }

            // The remainder can only be the empty pair or a symbol since argument list has been verified above.
            if (scheme_element_is_type(argumentList, scheme_symbol_get_type()))
            {
                restID = scheme_symbol_get_value((scheme_symbol *)argumentList);
            }
            else
            {
                restID = NULL;
            }
        }
        else
        {
            // Argument list is empty.
            arguments_arr = NULL;
            restID = NULL;
        }
    }

    // Get list of expressions.
    expressions_arr = scheme_list_to_array((scheme_pair *)expressions, &expressionCount);

    scheme_lambda *procedure = scheme_lambda_new(name, arguments_arr, argumentCount, restID, expressions_arr, expressionCount);

    free(expressions_arr);
    free(restID);

    for (int i = 0; i < argumentCount; ++i)
    {
        free(arguments_arr[i].id);
    }
    free(arguments_arr);

    return procedure;
}

scheme_element_type *scheme_lambda_get_type()
{
    if (!_scheme_lambda_type_initd)
    {
        _scheme_lambda_type.super = scheme_procedure_get_type();

        _scheme_lambda_type_initd = 1;
    }

    return &_scheme_lambda_type;
}
