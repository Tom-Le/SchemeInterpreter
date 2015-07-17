/**
 * Scheme boolean symbol.
 *
 * This type is compatible with scheme_element.
 * You may cast any scheme_boolean pointer to scheme_element and pass it to
 * any function that accepts a scheme_element pointer.
 */

#ifndef __SCHEME_BOOLEAN_H__
#define __SCHEME_BOOLEAN_H__

#include "scheme-element.h"

// Scheme boolean symbol values.
enum scheme_boolean_value {
    SCHEME_BOOLEAN_VALUE_TRUE,
    SCHEME_BOOLEAN_VALUE_FALSE
};

// Scheme boolean symbol.
typedef struct scheme_boolean scheme_boolean;

/**
 * Get #t symbol.
 *
 * The returned pointer does not need to be freed with scheme_element_free().
 * You may safely pass it as a parameter to scheme_element_free() as it will
 * do nothing.
 *
 * You cannot make copies of a Scheme boolean symbol. If you pass this pointer
 * to scheme_element_copy(), it will return the same pointer.
 *
 * @return #t symbol.
 */
scheme_boolean *scheme_boolean_get_true();

/**
 * Get #f symbol.
 *
 * The returned pointer does not need to be freed with scheme_element_free().
 * You may safely pass it as a parameter to scheme_element_free() as it will
 * do nothing.
 *
 * You cannot make copies of a Scheme boolean symbol. If you pass this pointer
 * to scheme_element_copy(), it will return the same pointer.
 *
 * @return #f symbol.
 */
scheme_boolean *scheme_boolean_get_false();

/**
 * Get boolean symbol's value.
 *
 * @param  symbol  A boolean symbol.
 *
 * @return Symbol's value.
 */
enum scheme_boolean_value scheme_boolean_get_value(scheme_boolean *symbol);

/**
 * Get boolean symbol's type.
 *
 * @return Boolean symbol's type.
 */
scheme_element_type *scheme_boolean_get_type();

#endif
