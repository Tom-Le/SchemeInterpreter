#include <stdlib.h>

#include "eval.h"
#include "procedure-utils.h"
#include "scheme-data-types.h"
#include "scheme-procedure-init.h"

#include "procedure-quote.h"

/**** Private variables ****/

static scheme_procedure _procedure_quote;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "quote".
 * Return the supplied argument as-is.
 *
 * Will return NULL if:
 *   - Supplied element is not in the format (<element>).
 *   - Out of memory.
 *
 * @param  element  A Scheme element.
 *
 * @return Supplied element as-is.
 */
static scheme_element *_quote_function(scheme_element *element);

/**** Private function implementations ****/

static scheme_element *_quote_function(scheme_element *element)
{
    // Get arguments.
    int argCount;
    scheme_element **args = procedure_get_arguments(element, &argCount);

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

    // Simply return a copy of argument.
    return scheme_element_copy(arg);
}

scheme_procedure *scheme_procedure_quote()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_quote, PROCEDURE_QUOTE_NAME, _quote_function);
        _proc_initd = 1;
    }

    return &_procedure_quote;
}
