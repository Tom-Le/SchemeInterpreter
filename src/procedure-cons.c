#include <stdlib.h>

#include "eval.h"
#include "procedure-utils.h"
#include "scheme-data-types.h"
#include "scheme-procedure-init.h"

#include "procedure-cons.h"

/**** Private variables ****/

static scheme_procedure _procedure_cons;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "cons".
 * Return a Scheme pair with first and second elements set to the given
 * arguments.
 *
 * Will return NULL if:
 *   - Supplied element is not a pair in the format: (<element> <element>)
 *   - Out of memory.
 *
 * @param  element  A Scheme element.
 *
 * @return Constructed pair, or NULL if an error occurred.
 */
static scheme_element *_cons_function(scheme_element *element);

/**** Private function implementations ****/

static scheme_element *_cons_function(scheme_element *element)
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

    scheme_element *firstArg = *args;
    scheme_element *secondArg = *(args+1);
    free(args);

    // Evaluate first and second arguments.
    firstArg = scheme_evaluate(firstArg);
    secondArg = scheme_evaluate(secondArg);
    if (firstArg == NULL || secondArg == NULL)
    {
        scheme_element_free(firstArg);
        scheme_element_free(secondArg);
        return NULL;
    }

    return (scheme_element *)scheme_pair_new(firstArg, secondArg);
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_cons()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_cons, PROCEDURE_CONS_NAME, _cons_function);
        _proc_initd = 1;
    }

    return &_procedure_cons;
}
