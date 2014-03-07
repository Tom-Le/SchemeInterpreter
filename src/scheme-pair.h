/**
 * Scheme pair.
 * A Scheme pair holds exactly two elements.
 * It is not mutable.
 *
 * This type is compatible with scheme_element.
 * You may cast any scheme_pair pointer to scheme_element and pass it to
 * any function that accepts a scheme_element pointer.
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
 *
 * Returned pointer must be freed with scheme_element_free(). When passed to
 * this function, the pair's first and second function will also be freed.
 *
 * If you would like to retain a pair's first and/or second elements before
 * freeing the pair, consider making a copy of the those elements using
 * scheme_element_copy().
 *
 * If you make a copy of a pair using scheme_element_copy(), you will receive
 * a deep copy, with first and second elements also being copies of the original
 * pair's elements. If you free the original pair, the copy will be unaffected,
 * and vice versa.
 *
 * @param  first   Pair's first element.
 * @param  second  Pair's second element.
 *
 * @return Newly created pair, or NULL if out of memory.
 */
scheme_pair *scheme_pair_new(scheme_element *first, scheme_element *second);

/**
 * Get an empty pair with no first or second element.
 *
 * Returned pointer cannot be freed with scheme_element_free(). You may
 * safely pass it as a parameter to scheme_element_free(), however, as
 * it will do nothing.
 *
 * You also cannot make a copy of the empty pair. If the function
 * scheme_element_copy() is given the pointer to the empty pair, it will
 * return the same pointer.
 *
 * @return The empty pair.
 */
scheme_pair *scheme_pair_get_empty();

/**
 * Check if pair is empty.
 *
 * You may also directly compare the given pointer to the pointer
 * returned by scheme_pair_get_empty().
 *
 * @return 1 if pair is empty, 0 otherwise.
 */
int scheme_pair_is_empty(scheme_pair *pair);

/**
 * Get pair's first element.
 *
 * @param  pair  A Scheme pair.
 *
 * @return A Scheme element or NULL if pair is empty.
 */
scheme_element *scheme_pair_get_first(scheme_pair *pair);

/**
 * Get pair's second element.
 *
 * @param  pair  A Scheme pair.
 *
 * @return A Scheme element or NULL if pair is empty.
 */
scheme_element *scheme_pair_get_second(scheme_pair *pair);

#endif
