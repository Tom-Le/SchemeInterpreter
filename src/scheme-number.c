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
 * Will return 0 if either pointer is not a number symbol.
 *
 * @param  element  Should be a Scheme number.
 * @param  other    Other element.
 *
 * @return 1 if equal, 0 otherwise.
 */
static int _vtable_compare(scheme_element *element, scheme_element *other);

/**** Private variables ****/

// Global virtual function table.
static struct scheme_element_vtable _scheme_number_vtable = {
    .get_type = scheme_number_get_type,
    .free = _vtable_free,
    .print = _vtable_print,
    .copy = _vtable_copy,
    .compare = _vtable_compare
};

// Global number symbol type.
static struct scheme_element_type _scheme_number_type = {
    .super = NULL,
    .name = "scheme_number"
};
static int _scheme_number_type_initd = 0;

/**** Private function implementations ****/

static void _vtable_free(scheme_element *element)
{
    free(element);
}

static void _vtable_print(scheme_element *element)
{
    scheme_number *symbol = (scheme_number *)element;
    printf("%d\n", symbol->value);
}

static scheme_element *_vtable_copy(scheme_element *element)
{
    scheme_number *num = (scheme_number *)element;
    return (scheme_element *)scheme_number_new(num->value);
}

static int _vtable_compare(scheme_element *element, scheme_element *other)
{
    if (!scheme_element_is_type(other, &_scheme_number_type)) return 0;

    return ((scheme_number *)element)->value == ((scheme_number *)other)->value;
}

/**** Public function implementations ****/

scheme_number *scheme_number_new(int value)
{
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

scheme_element_type *scheme_number_get_type()
{
    if (!_scheme_number_type_initd)
    {
        _scheme_number_type.super = scheme_element_get_base_type();

        _scheme_number_type_initd = 1;
    }

    return &_scheme_number_type;
}
