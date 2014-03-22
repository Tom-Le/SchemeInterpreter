#include <string.h>

#include "scheme-data-types.h"
#include "scheme-element.h"
#include "scheme-element-private.h"

// For scheme_element struct and its virtual function table,
// please check scheme-element-private.h.

/**** Private function declarations ****/

/**
 * Check if first argument is the empty pair and second argument
 * is the #f symbol.
 *
 * Convenience function used in scheme_element_compare().
 *
 * @param  first   A Scheme element.
 * @param  second  A Scheme element.
 *
 * @return 1 if first argument is the empty pair and second argument
 *         is the #f symbol, 0 otherwise.
 */
static inline int _empty_and_false(scheme_element *first, scheme_element *second);

/**** Private variables ****/

// Global type for generic Scheme elements.
static struct scheme_element_type _scheme_element_type = {
    .super = NULL,
    .name = "scheme_element"
};

static inline int _empty_and_false(scheme_element *first, scheme_element *second)
{
    return scheme_element_is_type(first, scheme_pair_get_type())
        && scheme_pair_is_empty((scheme_pair *)first)
        && scheme_element_is_type(second, scheme_boolean_get_type())
        && scheme_boolean_get_value((scheme_boolean *)second) == SCHEME_BOOLEAN_VALUE_FALSE;
}

/**** Implementations of public functions from scheme-element.h ****/

scheme_element_type *scheme_element_get_type(scheme_element *element)
{
    if (element == NULL) return NULL;
    return element->vtable->get_type();
}

void scheme_element_free(scheme_element *element)
{
    if (element == NULL) return;
    element->vtable->free(element);
}

void scheme_element_print(scheme_element *element)
{
    if (element == NULL) return;
    element->vtable->print(element);
}

scheme_element *scheme_element_copy(scheme_element *element)
{
    if (element == NULL) return NULL;
    return element->vtable->copy(element);
}

int scheme_element_compare(scheme_element *element, scheme_element *other)
{
    if (element == NULL) return 0;
    if (other == NULL) return 0;

    // Special case: If one element is #f and the other is '(), consider them equal.
    if (_empty_and_false(element, other) || _empty_and_false(other, element))
        return 1;

    return element->vtable->compare(element, other);
}

scheme_element_type *scheme_element_get_base_type()
{
    return &_scheme_element_type;
}

int scheme_element_is_type(scheme_element *element, scheme_element_type *type)
{
    if (element == NULL) return 0;

    scheme_element_type *elementType = scheme_element_get_type(element);
    while (elementType != NULL)
    {
        if (elementType == type)
        {
            return 1;
        }

        elementType = elementType->super;
    }

    return 0;
}

/**** Implementations of public functions from scheme-element-private.h ****/

void scheme_element_vtable_clone(struct scheme_element_vtable *target, struct scheme_element_vtable *source)
{
    target->get_type = source->get_type;
    target->free = source->free;
    target->print = source->print;
    target->copy = source->copy;
    target->compare = source->compare;
}
