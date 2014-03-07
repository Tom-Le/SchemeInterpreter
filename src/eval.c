#include <stdlib.h>
#include <string.h>

#include "procedures.h"
#include "eval.h"

/**** Private variables ****/

static scheme_procedure *_builtins[PROCEDURE_COUNT];

static int _static_initialized = 0;

/**** Private function declarations ****/

/**
 * Initialize built-in Scheme procedures.
 */
static void _init_builtin();

/**
 * Search for a built-in Scheme procedure.
 *
 * @param  name  Procedure's name.
 *
 * @return Procedure, or NULL if no built-in procedure exists with
 *     given name.
 */
static scheme_procedure *_search_procedure(char *name);

/**** Private function implementations ****/

static void _init_builtin()
{
    if (_static_initialized)
        return;

    // Initialize jump table.
    _builtins[0] = scheme_procedure_car();
    _builtins[1] = scheme_procedure_quote();
    _builtins[2] = scheme_procedure_cdr();
    _builtins[3] = scheme_procedure_symbol();
    _builtins[4] = scheme_procedure_cons();

    _static_initialized = 1;
}

static scheme_procedure *_search_procedure(char *name)
{
    for (int i = 0; i < PROCEDURE_COUNT; ++i)
    {
        scheme_procedure *proc = _builtins[i];
        char *procName = scheme_procedure_get_name(proc);
        int nameCmp = strcmp(name, procName);
        free(procName);

        if (nameCmp == 0)
            return proc;
    }

    return NULL;
}

/**** Public function implementations ****/

scheme_element *scheme_evaluate(scheme_element *element)
{
    _init_builtin();

    if (element == NULL)
        return NULL;

    // If element is a symbol, resolve it.
    if (scheme_element_is_type(element, SCHEME_SYMBOL_TYPE))
    {
        char *symbolValue = scheme_symbol_get_value((scheme_symbol *)element);

        // TODO: Local definitions, etc.

        // Check if there is a procedure of given name.
        scheme_procedure *proc = _search_procedure(symbolValue);
        free(symbolValue);
        if (proc != NULL)
            return scheme_element_copy((scheme_element *)proc);

        return scheme_element_copy(element);
    }

    // If element is not a pair, simply return it.
    if (!scheme_element_is_type(element, SCHEME_PAIR_TYPE))
        return scheme_element_copy(element);

    // Element is a pair.
    // Evaluate first element.
    scheme_pair *pair = (scheme_pair *)element;
    scheme_element *first = scheme_pair_get_first(pair);;
    first = scheme_evaluate(first);

    // Evaluated first element must be a procedure.
    if (!scheme_element_is_type(first, SCHEME_PROCEDURE_TYPE))
    {
        scheme_element_free(first);
        return scheme_element_copy(element);
    }

    // Evaluate expression using Scheme procedure and return result.
    scheme_element *second = scheme_pair_get_second(pair);
    scheme_element *result = scheme_procedure_apply((scheme_procedure *)first, second);
    scheme_element_free(first);
    return result;
}
