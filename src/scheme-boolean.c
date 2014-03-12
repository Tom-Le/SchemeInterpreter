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
static void _static_init();

// Documentation for virtual functions can be found in header file.

/**
 * Return number symbol's type identifier string.
 *
 * @return Type identifier string.
 */
static char *_vtable_get_type();

/**
 * Scheme boolean symbols are implemented as static variables and
 * cannot be freed. This function does nothing.
 *
 * @param  element  Should be a Scheme boolean symbol.
 */
static void _vtable_free(scheme_element *element);

/**
 * Print boolean symbol to stdout.
 *
 * @param  element  Should be a Scheme boolean symbol.
 */
static void _vtable_print(scheme_element *element);

/**
 * Scheme boolean symbols are implemented as static variables and
 * cannot be copied. This function simply returns its parameter.
 *
 * @param  element  Should be a Scheme boolean symbol.
 *
 * @return Given parameter as-is.
 */
static scheme_element *_vtable_copy(scheme_element *element);

/**
 * Compare boolean symbol to another symbol.
 * Will return 0 if either pointers are not boolean symbols.
 *
 * @param  element  Should be a Scheme boolean symbol.
 * @param  other    A Scheme element.
 *
 * @return 1 if equal, 0 otherwise.
 */
static int _vtable_compare(scheme_element *element, scheme_element *other);

/**** Private variables ****/

// Static variables for #t and #f symbols.
static struct scheme_boolean _scheme_boolean_true;
static struct scheme_boolean _scheme_boolean_false;

// Global virtual function table.
static struct scheme_element_vtable _scheme_boolean_vtable;

static int _static_initialized = 0;

/**** Private function implementations ****/

static void _static_init()
{
    if (!_static_initialized)
    {
        _scheme_boolean_vtable.get_type = _vtable_get_type;
        _scheme_boolean_vtable.free = _vtable_free;
        _scheme_boolean_vtable.print = _vtable_print;
        _scheme_boolean_vtable.copy = _vtable_copy;
        _scheme_boolean_vtable.compare = _vtable_compare;

        _scheme_boolean_true.super.vtable = &_scheme_boolean_vtable;
        _scheme_boolean_true.value = SCHEME_BOOLEAN_VALUE_TRUE;

        _scheme_boolean_false.super.vtable = &_scheme_boolean_vtable;
        _scheme_boolean_false.value = SCHEME_BOOLEAN_VALUE_FALSE;

        _static_initialized = 1;
    }
}

static char *_vtable_get_type()
{
    return SCHEME_BOOLEAN_TYPE;
}

static void _vtable_free(scheme_element *element)
{
}

static void _vtable_print(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_BOOLEAN_TYPE))
        return;

    scheme_boolean *symbol = (scheme_boolean *)element;

    if (symbol->value == SCHEME_BOOLEAN_VALUE_TRUE)
        printf("#t");
    else
        printf("#f");
}

static scheme_element *_vtable_copy(scheme_element *element)
{
    return element;
}

static int _vtable_compare(scheme_element *element, scheme_element *other)
{
    if (!scheme_element_is_type(element, SCHEME_BOOLEAN_TYPE)) return 0;
    if (!scheme_element_is_type(other, SCHEME_BOOLEAN_TYPE)) return 0;

    // Boolean symbols are static, only need to compare pointers.
    return element == other;
}

/**** Public function implementations ****/

scheme_boolean *scheme_boolean_get_true()
{
    _static_init();
    return &_scheme_boolean_true;
}

scheme_boolean *scheme_boolean_get_false()
{
    _static_init();
    return &_scheme_boolean_false;
}

enum scheme_boolean_value scheme_boolean_get_value(scheme_boolean *symbol)
{
    return symbol->value;
}
