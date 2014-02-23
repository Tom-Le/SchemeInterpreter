/**
 * Scheme boolean symbol.
 *
 * This type is compatible with scheme_element.
 * You may pass any scheme_boolean pointer as a scheme_element pointer.
 */

#ifndef __SCHEME_BOOLEAN_H__
#define __SCHEME_BOOLEAN_H__

#include "scheme-element.h"

// Scheme boolean symbol type.
#define SCHEME_BOOLEAN_TYPE "SCHEME_BOOLEAN_TYPE"

// Scheme boolean symbol values.
enum scheme_boolean_value {
    SCHEME_BOOLEAN_VALUE_TRUE,
    SCHEME_BOOLEAN_VALUE_FALSE
};

// Scheme boolean symbol.
typedef struct scheme_boolean scheme_boolean;

/**
 * Get #t symbol.
 * This symbol does not need to be freed.
 * scheme_element_free() will do nothing if it receives this symbol.
 */
scheme_boolean *scheme_boolean_get_true();

/**
 * Get #f symbol.
 * This symbol does not need to be freed.
 * scheme_element_free() will do nothing if it receives this symbol.
 */
scheme_boolean *scheme_boolean_get_false();

/**
 * Get boolean symbol's value.
 *
 * @param  symbol  A boolean symbol.
 */
enum scheme_boolean_value scheme_boolean_get_value(scheme_boolean *symbol);

#endif
