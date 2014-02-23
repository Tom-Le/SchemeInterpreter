/**
 * Scheme number symbol.
 *
 * This type is compatible with scheme_element.
 * You may pass any scheme_number pointer as a scheme_element pointer.
 */

#ifndef __SCHEME_NUMBER_H__
#define __SCHEME_NUMBER_H__

#include "scheme-element.h"

// Scheme number symbol type.
#define SCHEME_NUMBER_TYPE "SCHEME_NUMBER_TYPE"

// Scheme number symbol.
typedef struct scheme_number scheme_number;

/**
 * Create new Scheme number symbol.
 * Symbol must be freed afterwards with scheme_element_free().
 *
 * @param  value  Symbol's initial value.
 *
 * @return Newly created symbol, or NULL if out of memory.
 */
scheme_number *scheme_number_new(int value);

/**
 * Get number symbol's value.
 *
 * @param  symbol  A number symbol.
 */
int scheme_number_get_value(scheme_number *symbol);

#endif
