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
 * Initialize static variables.
 */
static void _static_init();

/**
 * Return pair's type identifier string.
 *
 * @param  element  Should be a Scheme pair.
 *
 * @return Type identifier.
 */
static char *_vtable_get_type();

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

// Static variables for empty list.
static struct scheme_pair _empty_pair;

// Global virtual function table.
static struct scheme_element_vtable _scheme_pair_vtable;

static int _static_initialized = 0;

/**** Private function implementations ****/

static void _static_init()
{
    if (!_static_initialized)
    {
        _scheme_pair_vtable.get_type = _vtable_get_type;
        _scheme_pair_vtable.free = _vtable_free;
        _scheme_pair_vtable.print = _vtable_print;
        _scheme_pair_vtable.copy = _vtable_copy;
        _scheme_pair_vtable.compare = _vtable_compare;

        _empty_pair.super.vtable = &_scheme_pair_vtable;
        _empty_pair.first = NULL;
        _empty_pair.second = NULL;

        _static_initialized = 1;
    }
}

static char *_vtable_get_type()
{
    return SCHEME_PAIR_TYPE;
}

static void _vtable_free(scheme_element *element)
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

static scheme_element *_vtable_copy(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_PAIR_TYPE))
        return NULL;

    scheme_pair *pair = (scheme_pair *)element;

    // Do not copy the empty pair.
    if (pair == &_empty_pair)
        return element;

    // Make copies of its elements.
    scheme_element *firstCopy = scheme_element_copy(pair->first);
    scheme_element *secondCopy = scheme_element_copy(pair->second);

    return (scheme_element *)scheme_pair_new(firstCopy, secondCopy);
}

static void _vtable_print(scheme_element *element)
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
    _do_print(pair);
    putchar(')');
}

static void _do_print(scheme_pair *pair)
{
    // Print first element.
    scheme_element_print(pair->first);

    // If second element is a pair, print it in condensed form.
    // Else, use dot syntax.
    if (scheme_element_is_type(pair->second, SCHEME_PAIR_TYPE))
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
    if (!scheme_element_is_type(element, SCHEME_PAIR_TYPE)) return 0;
    if (!scheme_element_is_type(other, SCHEME_PAIR_TYPE)) return 0;

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
    _static_init();

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
    _static_init();
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
