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
 * Initialize static variables.
 */
static void _static_init();

/**
 * Return void symbol's type identifier.
 *
 * @return Type identifier string.
 */
static char *_vtable_get_type();

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

// Static variable for void symbol.
static struct scheme_void _scheme_void_symbol;

// Global virtual function table.
static struct scheme_element_vtable _scheme_void_vtable;

static int _static_initialized = 0;

/**** Private function implementation ****/

static void _static_init()
{
    if (!_static_initialized)
    {
        _scheme_void_vtable.copy = _vtable_copy;
        _scheme_void_vtable.free = _vtable_free;
        _scheme_void_vtable.print = _vtable_print;
        _scheme_void_vtable.compare = _vtable_compare;
        _scheme_void_vtable.get_type = _vtable_get_type;

        _scheme_void_symbol.super.vtable = &_scheme_void_vtable;

        _static_initialized = 1;
    }
}

static char *_vtable_get_type()
{
    return SCHEME_VOID_TYPE;
}

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
    if (!scheme_element_is_type(element, SCHEME_VOID_TYPE)) return 0;
    if (!scheme_element_is_type(other, SCHEME_VOID_TYPE)) return 0;

    return element == other;
}

/**** Public function implementations ****/

scheme_element *scheme_void_get()
{
    _static_init();
    return (scheme_element *)&_scheme_void_symbol;
}
