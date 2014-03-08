#include <stdio.h>
#include <stdlib.h>

#include "scheme-number.h"
#include "scheme-element-private.h"

// Scheme number symbol.
struct scheme_number {
    struct scheme_element super;
    int value;
};

/**** Private function declarations ****/

/**
 * Initialize static variables.
 */
static void _static_init();

/**
 * Return number symbol's type identifier.
 *
 * @param  element  Should be a Scheme number symbol.
 *
 * @return Type identifier.
 */
static char *_vtable_get_type();

/**
 * Free Scheme number.
 *
 * @param  element  Should be a Scheme number symbol.
 */
static void _vtable_free(scheme_element *element);

/**
 * Print Scheme number symbol to stdout.
 *
 * @param  element  Should be a Scheme number symbol.
 */
static void _vtable_print(scheme_element *element);

/**
 * Copy number symbol.
 *
 * @param  element  Should be a Scheme number symbol.
 *
 * @return A copy, or NULL if out of memory.
 */
static scheme_element *_vtable_copy(scheme_element *element);

/**
 * Compare number symbol to another symbol.
 * Will return 0 if either pointers are not number symbols.
 *
 * @param  element  Should be a Scheme number.
 * @param  other    Other element.
 *
 * @return 1 if equal, 0 otherwise.
 */
static int _vtable_compare(scheme_element *element, scheme_element *other);

/**** Private variables ****/

// Global virtual function table.
static struct scheme_element_vtable _scheme_number_vtable;

static int _static_initialized = 0;

/**** Private function implementations ****/

static void _static_init()
{
    if (!_static_initialized)
    {
        _scheme_number_vtable.get_type = _vtable_get_type;
        _scheme_number_vtable.free = _vtable_free;
        _scheme_number_vtable.print = _vtable_print;
        _scheme_number_vtable.copy = _vtable_copy;
        _scheme_number_vtable.compare = _vtable_compare;

        _static_initialized = 1;
    }
}

static char *_vtable_get_type()
{
    return SCHEME_NUMBER_TYPE;
}

static void _vtable_free(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_NUMBER_TYPE))
        return;

    free(element);
}

static void _vtable_print(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_NUMBER_TYPE))
        return;

    scheme_number *symbol = (scheme_number *)element;
    printf("%d", symbol->value);
}

static scheme_element *_vtable_copy(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_NUMBER_TYPE))
        return NULL;

    scheme_number *num = (scheme_number *)element;
    return (scheme_element *)scheme_number_new(num->value);
}

static int _vtable_compare(scheme_element *element, scheme_element *other)
{
    if (!scheme_element_is_type(element, SCHEME_NUMBER_TYPE)) return 0;
    if (!scheme_element_is_type(other, SCHEME_NUMBER_TYPE)) return 0;

    return ((scheme_number *)element)->value == ((scheme_number *)other)->value;
}

/**** Public function implementations ****/

scheme_number *scheme_number_new(int value)
{
    _static_init();

    // Allocate symbol.
    scheme_number *symbol;
    if ((symbol = malloc(sizeof(scheme_number))) == NULL)
        return NULL;

    // Set up virtual function table.
    symbol->super.vtable = &_scheme_number_vtable;

    symbol->value = value;

    return symbol;
}

int scheme_number_get_value(scheme_number *symbol)
{
    return symbol->value;
}
