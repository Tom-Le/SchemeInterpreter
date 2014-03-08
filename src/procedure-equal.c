#include <stdlib.h>
#include <string.h>

#include "eval.h"
#include "procedure-utils.h"
#include "scheme-data-types.h"
#include "scheme-procedure-init.h"

#include "procedure-equal.h"

/**** Private variables ****/

static scheme_procedure _procedure_equal;

static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "equal?".
 * Compare two Scheme elements given as argument.
 *
 * Will return NULL if:
 *   - Supplied element is not a pair in the format (<element> <element>).
 *   - Out of memory.
 *
 * @param  element  A Scheme element.
 *
 * @return Scheme boolean #t if two elements are equal, #f if not, or NULL
 *   if an error occurred.
 */
static scheme_element *_equal_function(scheme_element *element);

/**
 * Check if first argument is the empty pair and second argument
 * is the #f symbol.
 *
 * Convenience function used in _equal_function().
 *
 * @param  first  A Scheme element.
 * @param  second  A Scheme element.
 *
 * @return 1 if first argument is the empty pair and second argument
 *   is the #f symbol, 0 otherwise.
 */
static inline int _empty_and_false(scheme_element *first, scheme_element *second);

/**** Private function implementations ****/

static scheme_element *_equal_function(scheme_element *element)
{
    // Get arguments.
    int argCount;
    scheme_element **args = procedure_get_arguments(element, &argCount);

    // Check if argument list is invalid.
    if (argCount == -1) return NULL;
    else if (argCount != 2)
    {
        // Wrong number of arguments.
        if (args != NULL) free(args);
        return NULL;
    }

    scheme_element *firstArg = *(args);
    scheme_element *secondArg = *(args+1);
    free(args);

    // Evaluate arguments.
    firstArg = scheme_evaluate(firstArg);
    secondArg = scheme_evaluate(secondArg);
    if (firstArg == NULL || secondArg == NULL)
    {
        scheme_element_free(firstArg);
        scheme_element_free(secondArg);
        return NULL;
    }

    // Special treatment: #f == ()
    if (_empty_and_false(firstArg, secondArg) || _empty_and_false(secondArg, firstArg))
    {
        // No need to free either of them.
        return (scheme_element *)scheme_boolean_get_true();
    }

    char *firstType = scheme_element_get_type(firstArg);
    char *secondType = scheme_element_get_type(secondArg);

    // Elements are not equal if they are not of the same type.
    if (strcmp(firstType, secondType) != 0)
    {
        scheme_element_free(firstArg);
        scheme_element_free(secondArg);
        return (scheme_element *)scheme_boolean_get_false();
    }

    int comparison = scheme_element_compare(firstArg, secondArg);
    scheme_element_free(firstArg);
    scheme_element_free(secondArg);

    return (scheme_element *)(comparison ? scheme_boolean_get_true() : scheme_boolean_get_false());
}

static inline int _empty_and_false(scheme_element *first, scheme_element *second)
{
    return scheme_element_is_type(first, SCHEME_PAIR_TYPE)
        && scheme_pair_is_empty((scheme_pair *)first)
        && scheme_element_is_type(second, SCHEME_BOOLEAN_TYPE)
        && scheme_boolean_get_value((scheme_boolean *)second) == SCHEME_BOOLEAN_VALUE_FALSE;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_equal()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_equal, PROCEDURE_EQUAL_NAME, _equal_function);
        _proc_initd = 1;
    }

    return &_procedure_equal;
}
