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

// Global virtual function table.
static struct scheme_element_vtable _scheme_boolean_vtable = {
    .get_type = scheme_boolean_get_type,
    .free = _vtable_free,
    .print = _vtable_print,
    .copy = _vtable_copy,
    .compare = _vtable_compare
};

// Static variables for #t and #f symbols.
static struct scheme_boolean _scheme_boolean_true = {
    .super.vtable = &_scheme_boolean_vtable,
    .value = SCHEME_BOOLEAN_VALUE_TRUE
};

static struct scheme_boolean _scheme_boolean_false = {
    .super.vtable = &_scheme_boolean_vtable,
    .value = SCHEME_BOOLEAN_VALUE_FALSE
};

// Global boolean symbol type.
static struct scheme_element_type _scheme_boolean_type = {
    .super = NULL,
    .name = "scheme_boolean"
};
static int _scheme_boolean_type_initd = 0;

/**** Private function implementations ****/

static void _vtable_free(scheme_element *element) {}

static void _vtable_print(scheme_element *element)
{
    scheme_boolean *symbol = (scheme_boolean *)element;

    if (symbol->value == SCHEME_BOOLEAN_VALUE_TRUE)
        printf("#t\n");
    else
        printf("#f\n");
}

static scheme_element *_vtable_copy(scheme_element *element)
{
    return element;
}

static int _vtable_compare(scheme_element *element, scheme_element *other)
{
    if (!scheme_element_is_type(other, &_scheme_boolean_type)) return 0;

    // Boolean symbols are static, only need to compare pointers.
    return element == other;
}

/**** Public function implementations ****/

scheme_boolean *scheme_boolean_get_true()
{
    return &_scheme_boolean_true;
}

scheme_boolean *scheme_boolean_get_false()
{
    return &_scheme_boolean_false;
}

enum scheme_boolean_value scheme_boolean_get_value(scheme_boolean *symbol)
{
    return symbol->value;
}

scheme_element_type *scheme_boolean_get_type()
{
    if (!_scheme_boolean_type_initd)
    {
        _scheme_boolean_type.super = scheme_element_get_base_type();

        _scheme_boolean_type_initd = 1;
    }

    return &_scheme_boolean_type;
}
