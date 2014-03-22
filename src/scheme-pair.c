#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * Free pair. Will also free its first and second elements.
 *
 * @param  element  Should be a Scheme pair.
 */
static void _vtable_free(scheme_element *element);

/**
 * Copy pair.
 *
 * @param  element  Should be a Scheme pair.
 *
 * @return Copy or NULL if out of memory.
 */
static scheme_element *_vtable_copy(scheme_element *element);

/**
 * Print pair to stdout.
 *
 * @param  element  Should be a Scheme pair.
 */
static void _vtable_print(scheme_element *element);

/**
 * Recursively print pair and its elements to stdout.
 * Used in _vtable_print().
 *
 * @param  element  A Scheme pair.
 */
static void _do_print(scheme_pair *pair);

/**
 * Compare a Scheme pair to another pair.
 * Will return 0 if either pointer is not a Scheme pair.
 *
 * @param  element  Should be a Scheme pair.
 * @param  other    A Scheme element.
 *
 * @return 1 if equal, 0 otherwise.
 */
static int _vtable_compare(scheme_element *element, scheme_element *other);

/**** Private variables ****/

// Global virtual function table.
static struct scheme_element_vtable _scheme_pair_vtable = {
    .get_type = scheme_pair_get_type,
    .free = _vtable_free,
    .print = _vtable_print,
    .copy = _vtable_copy,
    .compare = _vtable_compare
};

// Static variables for empty list.
static struct scheme_pair _empty_pair = {
    .super.vtable = &_scheme_pair_vtable,
    .first = NULL,
    .second = NULL
};

// Global pair type.
static struct scheme_element_type _scheme_pair_type = {
    .super = NULL,
    .name = "scheme_pair"
};
static int _scheme_pair_type_initd = 0;

/**** Private function implementations ****/

static void _vtable_free(scheme_element *element)
{
    scheme_pair *pair = (scheme_pair *)element;

    // Do not free empty pair.
    if (pair == &_empty_pair) return;

    scheme_element_free(pair->first);
    scheme_element_free(pair->second);
    free(pair);
}

static scheme_element *_vtable_copy(scheme_element *element)
{
    scheme_pair *pair = (scheme_pair *)element;

    // Do not copy the empty pair.
    if (pair == &_empty_pair)
        return element;

    return (scheme_element *)scheme_pair_new(pair->first, pair->second);
}

static void _vtable_print(scheme_element *element)
{
    // Special treatment for empty pair.
    if ((scheme_pair *)element == &_empty_pair)
    {
        printf("()");
        return;
    }

    scheme_pair *pair = (scheme_pair *)element;

    putchar('(');
    _do_print(pair);
    putchar(')');
    putchar('\n');
}

static void _do_print(scheme_pair *pair)
{
    // Print first element.
    scheme_element_print(pair->first);

    // If second element is a pair, print it in condensed form.
    // Else, use dot syntax.
    if (scheme_element_is_type(pair->second, &_scheme_pair_type))
    {
        if ((scheme_pair *)pair->second != &_empty_pair)
        {
            putchar(' ');
            _do_print((scheme_pair *)pair->second);
        }
    }
    else
    {
        printf(" . ");
        scheme_element_print(pair->second);
    }
}

static int _vtable_compare(scheme_element *element, scheme_element *other)
{
    if (!scheme_element_is_type(other, &_scheme_pair_type)) return 0;

    scheme_pair *this = (scheme_pair *)element;
    scheme_pair *that = (scheme_pair *)other;

    // Special treatment for empty pairs.
    if (scheme_pair_is_empty(this) && scheme_pair_is_empty(that))
        return 1;

    return scheme_element_compare(this->first, that->first)
        && scheme_element_compare(this->second, that->second);
}

/**** Public function implementations ****/

scheme_pair *scheme_pair_new(scheme_element *first, scheme_element *second)
{
    scheme_pair *pair;
    if ((pair = malloc(sizeof(scheme_pair))) == NULL)
        return NULL;

    pair->super.vtable = &_scheme_pair_vtable;
    pair->first = scheme_element_copy(first);
    pair->second = scheme_element_copy(second);

    return pair;
}

scheme_pair *scheme_pair_get_empty()
{
    return &_empty_pair;
}

int scheme_pair_is_empty(scheme_pair *pair)
{
    return pair == &_empty_pair;
}

scheme_element *scheme_pair_get_first(scheme_pair *pair)
{
    return pair->first;
}

scheme_element *scheme_pair_get_second(scheme_pair *pair)
{
    return pair->second;
}

scheme_element_type *scheme_pair_get_type()
{
    if (!_scheme_pair_type_initd)
    {
        _scheme_pair_type.super = scheme_element_get_base_type();

        _scheme_pair_type_initd = 1;
    }

    return &_scheme_pair_type;
}
