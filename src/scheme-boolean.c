#include <stdio.h>
#include <stdlib.h>

#include "scheme-boolean.h"
#include "scheme-element-private.h"

// Scheme boolean symbol.
struct scheme_boolean {
    struct scheme_element super;
    enum scheme_boolean_value value;
};

/**** Private function declarations ****/

/**
 * Initialize static variables.
 */
static void _scheme_boolean_init();

/**
 * Return number symbol's type identifier.
 *
 * @param  element  Should be a Scheme boolean symbol.
 */
static char *_scheme_boolean_get_type(scheme_element *element);

/**
 * Do not free boolean symbols.
 * This function does nothing.
 */
static void _scheme_boolean_free(scheme_element *element);

/**
 * Print boolean symbol to stdout.
 */
static void _scheme_boolean_print(scheme_element *element);

/**** Private variables ****/

// Static variables for #t and #f symbols.
static struct scheme_boolean _scheme_boolean_true;
static struct scheme_boolean _scheme_boolean_false;
static int _static_initialized = 0;

// Global virtual function table.
static struct scheme_element_vtable _scheme_boolean_vtable = {
    _scheme_boolean_get_type,
    _scheme_boolean_free,
    _scheme_boolean_print
};

/**** Private function implementations ****/

static void _scheme_boolean_init()
{
    if (!_static_initialized)
    {
        _scheme_boolean_true.super.vtable = &_scheme_boolean_vtable;
        _scheme_boolean_true.value = SCHEME_BOOLEAN_VALUE_TRUE;

        _scheme_boolean_false.super.vtable = &_scheme_boolean_vtable;
        _scheme_boolean_false.value = SCHEME_BOOLEAN_VALUE_FALSE;

        _static_initialized = 1;
    }
}

static char *_scheme_boolean_get_type(scheme_element *element)
{
    return SCHEME_BOOLEAN_TYPE;
}

static void _scheme_boolean_free(scheme_element *element) {}

static void _scheme_boolean_print(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_BOOLEAN_TYPE))
        return;

    scheme_boolean *symbol = (scheme_boolean *)element;

    if (symbol->value == SCHEME_BOOLEAN_VALUE_TRUE)
        printf("#t");
    else
        printf("#f");
}

/**** Public function implementations ****/

scheme_boolean *scheme_boolean_get_true()
{
    _scheme_boolean_init();
    return &_scheme_boolean_true;
}

scheme_boolean *scheme_boolean_get_false()
{
    _scheme_boolean_init();
    return &_scheme_boolean_false;
}

enum scheme_boolean_value scheme_boolean_get_value(scheme_boolean *symbol)
{
    return symbol->value;
}
