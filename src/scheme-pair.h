/**
 * Scheme pair.
 * A Scheme pair holds exactly two elements.
 * It is not mutable.
 *
 * This type is compatible with scheme_element.
 * You may pass any scheme_symbol_number pointer as a scheme_element pointer.
 */

#ifndef __SCHEME_PAIR_H__
#define __SCHEME_PAIR_H__

#include "scheme-element.h"

// Scheme pair type identifier.
#define SCHEME_PAIR_TYPE "SCHEME_PAIR_TYPE"

// Scheme pair.
typedef struct scheme_pair scheme_pair;

/**
 * Create new Scheme pair.
 * Pair must be freed afterwards by calling scheme_element_free().
 * When a pair is freed, its first and second elements are also freed.
 *
 * @param  first   Pair's first element.
 * @param  second  Pair's second element.
 *
 * @return Newly created pair, or NULL if out of memory.
 */
scheme_pair *scheme_pair_new(scheme_element *first, scheme_element *second);

/**
 * Return empty pair with no first or second element.
 * An empty pair cannot be freed.
 * When scheme_element_free() is called with an empty pair, nothing happens.
 */
scheme_pair *scheme_pair_get_empty();

/**
 * Check if pair is empty.
 */
int *scheme_pair_is_empty(scheme_pair *pair);

/**
 * Get pair's first element.
 *
 * @param  pair  A Scheme pair.
 *
 * @return A Scheme element. If pair is empty, return NULL.
 */
scheme_element *scheme_pair_get_first(scheme_pair *pair);

/**
 * Get pair's second element.
 *
 * @param  pair  A Scheme pair. If pair is empty, return NULL.
 */
scheme_element *scheme_pair_get_second(scheme_pair *pair);

#endif
