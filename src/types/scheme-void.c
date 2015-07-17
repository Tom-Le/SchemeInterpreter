#include <stdio.h>
#include <stdlib.h>

#include "scheme-void.h"
#include "scheme-element-private.h"

// Scheme void symbol.
struct scheme_void {
    struct scheme_element super;
};

/**** Private function declarations ****/

/**
 * Scheme void symbol is implemented as a static variable and cannot
 * be freed. This function does nothing.
 *
 * @param  element  Should be a Scheme void symbol.
 */
static void _vtable_free(scheme_element *element);

/**
 * A void symbol does not have any value so there is nothing to print.
 * This function does nothing.
 *
 * @param  element  Should be a Scheme void symbol.
 */
static void _vtable_print(scheme_element *element);

/**
 * A void symbol cannot be copied. This function returns the pointer
 * as-is.
 *
 * @param  element  Should be a Scheme void symbol.
 *
 * @return Given pointer as-is.
 */
static scheme_element *_vtable_copy(scheme_element *element);

/**
 * Compare void symbol to another symbol.
 *
 * @param  element  Should be a Scheme void symbol.
 * @param  other    A Scheme element.
 *
 * @return 1 if equal, 0 otherwise.
 */
static int _vtable_compare(scheme_element *element, scheme_element *other);

/**** Private variables ****/

// Global virtual function table.
static struct scheme_element_vtable _scheme_void_vtable = {
    .get_type = scheme_void_get_type,
    .free = _vtable_free,
    .print = _vtable_print,
    .copy = _vtable_copy,
    .compare = _vtable_compare
};

// Static variable for void symbol.
static struct scheme_void _scheme_void_symbol = {
    .super.vtable = &_scheme_void_vtable
};

// Static struct for void element's type.
static struct scheme_element_type _scheme_void_type = {
    .super = NULL,
    .name = "scheme_void"
};
static int _scheme_void_type_initd = 0;

/**** Private function implementation ****/

static void _vtable_free(scheme_element *element)
{
}

static void _vtable_print(scheme_element *element)
{
}

static scheme_element *_vtable_copy(scheme_element *element)
{
    return element;
}

static int _vtable_compare(scheme_element *element, scheme_element *other)
{
    if (!scheme_element_is_type(other, &_scheme_void_type)) return 0;

    return element == other;
}

/**** Public function implementations ****/

scheme_element *scheme_void_get()
{
    return (scheme_element *)&_scheme_void_symbol;
}

scheme_element_type *scheme_void_get_type()
{
    if (!_scheme_void_type_initd)
    {
        _scheme_void_type.super = scheme_element_get_base_type();

        _scheme_void_type_initd = 1;
    }

    return &_scheme_void_type;
}
