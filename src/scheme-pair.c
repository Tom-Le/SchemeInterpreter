#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "scheme-pair.h"
#include "scheme-element-private.h"

// Scheme pair.
struct scheme_pair {
    struct scheme_element super;
    scheme_element *first;
    scheme_element *second;
};

/**** Private function declarations ****/

/**
 * Initialize static variables.
 */
static void _init_static();

/**
 * Return pair's type identifier.
 */
static char *_scheme_pair_get_type(scheme_element *element);

/**
 * Free pair.
 */
static void _scheme_pair_free(scheme_element *element);

/**
 * Print pair to stdout.
 */
static void _scheme_pair_print(scheme_element *element);

/**
 * Print pair to stdout without parentheses.
 * Useful for printing a Scheme list.
 */
static void _scheme_pair_print_condensed(scheme_pair *pair);

/**
 * (Actually) pair to stdout.
 */
static void _scheme_pair_do_print(scheme_pair *pair);

/**** Private variables ****/

// Static variables for empty list.
static struct scheme_pair _empty_pair;
static int _static_initialized = 0;

// Global virtual function table.
static struct scheme_element_vtable _scheme_pair_vtable = {
    _scheme_pair_get_type,
    _scheme_pair_free,
    _scheme_pair_print
};

/**** Private function implementations ****/

static void _init_static()
{
    if (!_static_initialized)
    {
        _empty_pair.super.vtable = &_scheme_pair_vtable;
        _empty_pair.first = NULL;
        _empty_pair.second = NULL;

        _static_initialized = 1;
    }
}

static char *_scheme_pair_get_type(scheme_element *element)
{
    return SCHEME_PAIR_TYPE;
}

static void _scheme_pair_free(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_PAIR_TYPE))
        return;

    scheme_pair *pair = (scheme_pair *)element;

    // Do not free empty pair.
    if (pair == &_empty_pair) return;

    scheme_element_free(pair->first);
    scheme_element_free(pair->second);
    free(pair);
}

static void _scheme_pair_print(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_PAIR_TYPE))
        return;

    // Special treatment for empty pair.
    if ((scheme_pair *)element == &_empty_pair)
    {
        printf("()");
        return;
    }

    scheme_pair *pair = (scheme_pair *)element;

    putchar('(');
    _scheme_pair_do_print(pair);
    putchar(')');
}

static void _scheme_pair_print_condensed(scheme_pair *pair)
{
    // Special treatment for empty pair.
    if (pair == &_empty_pair) return;

    _scheme_pair_do_print(pair);
}

static void _scheme_pair_do_print(scheme_pair *pair)
{
    // Print first element.
    scheme_element_print(pair->first);

    // If second element is a pair, print it in condensed form.
    // Else, use dot syntax.
    if (scheme_element_is_type(pair->second, SCHEME_PAIR_TYPE))
    {
        if ((scheme_pair *)pair->second != &_empty_pair) putchar(' ');
        _scheme_pair_print_condensed((scheme_pair *)pair->second);
    }
    else
    {
        printf(" . ");
        scheme_element_print(pair->second);
    }
}

/**** Public function implementations ****/

scheme_pair *scheme_pair_new(scheme_element *first, scheme_element *second)
{
    scheme_pair *pair;
    if ((pair = malloc(sizeof(scheme_pair))) == NULL)
        return NULL;

    pair->super.vtable = &_scheme_pair_vtable;
    pair->first = first;
    pair->second = second;

    return pair;
}

scheme_pair *scheme_pair_get_empty()
{
    _init_static();
    return &_empty_pair;
}

scheme_element *scheme_pair_get_first(scheme_pair *pair)
{
    return pair->first;
}

scheme_element *scheme_pair_get_second(scheme_pair *pair)
{
    return pair->second;
}
