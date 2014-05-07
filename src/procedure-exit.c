#include <stdlib.h>

#include "main.h"
#include "eval.h"
#include "scheme-data-types.h"
#include "utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-exit.h"

/**** Private variables ****/

static scheme_procedure _procedure_exit;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "exit".
 *
 * Will return NULL if:
 * - Supplied argument is not a pair in the format: (<element>)
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Void symbol.
 */
static scheme_element *_exit_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * THis function does nothing.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_exit_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    // Get arguments.
    int argCount;
    scheme_element **args = scheme_list_to_array((scheme_pair *)element, &argCount);

    // Check if argument list is invalid.
    if (argCount == -1)
    {
        return NULL;
    }
    else if (argCount > 1)
    {
        free(args);
        return NULL;
    }

    if (argCount == 1)
    {
        scheme_element *arg = *args;
        free(args);

        // Evaluate argument.
        scheme_element *result = scheme_evaluate(arg, namespace);
        if (result == NULL) return NULL;

        // If evaluated result is a number, set the number as exit code.
        if (scheme_element_is_type(result, scheme_number_get_type()))
        {
            g_SchemeProgramTerminationCode = scheme_number_get_value((scheme_number *)result) % 256;
        }
    }

    // Signal main program to terminate.
    g_SchemeProgramTerminationFlag = 1;

    return (scheme_element *)scheme_void_get();
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_exit()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_exit, PROCEDURE_EXIT_NAME, _exit_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_exit.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_exit.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_exit;
}
