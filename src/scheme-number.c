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
 * Return number symbol's type identifier.
 *
 * @param  element  Should be a Scheme number symbol.
 */
static char *_scheme_number_get_type(scheme_element *element);

/**
 * Free Scheme number.
 *
 * @param  element  Should be a Scheme number symbol.
 */
static void _scheme_number_free(scheme_element *element);

/**
 * Print Scheme number symbol to stdout.
 */
static void _scheme_number_print(scheme_element *element);

/**** Private variables ****/

// Global virtual function table.
static struct scheme_element_vtable _scheme_number_vtable = {
    _scheme_number_get_type,
    _scheme_number_free,
    _scheme_number_print
};

/**** Private function implementations ****/

static char *_scheme_number_get_type(scheme_element *element)
{
    return SCHEME_NUMBER_TYPE;
}

static void _scheme_number_free(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_NUMBER_TYPE))
        return;

    free(element);
}

static void _scheme_number_print(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_NUMBER_TYPE))
        return;

    scheme_number *symbol = (scheme_number *)element;
    printf("%d", symbol->value);
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
