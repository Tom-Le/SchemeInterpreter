/**
 * Utility functions for Scheme pairs.
 */

#ifndef __SCHEME_PAIR_UTILS_H__
#define __SCHEME_PAIR_UTILS_H__

#include "scheme-data-types.h"

/**
 * Check if a Scheme pair is a list.
 *
 * @param  pair  A Scheme pair.
 *
 * @return 1 if pair is a list, 0 otherwise.
 */
int scheme_pair_is_list(scheme_pair *list);

/**
 * Convert a Scheme list to an array of Scheme elements in the list.
 *
 * Caller is responsible for freeing returned pointer with free().
 * This will not free elements referenced by the pointer, which are
 * still stored in the given Scheme list. To free those elements,
 * pass the list to scheme_element_free().
 *
 * Will return NULL if list is empty, with count set to 0.
 *
 * Will also return NULL if first argument is not a valid list or if
 * out of memory, with count set to -1.
 *
 * @param  list   A Scheme list.
 * @param  count  If not NULL, set to number of elements in the list.
 *
 * @return An array of Scheme elements in the list or NULL if list is
 *         empty or an error occurs.
 */
scheme_element **scheme_list_to_array(scheme_pair *list, int *count);

/**
 * From a Scheme list, create another Scheme list with every element
 * evaluated.
 *
 * @param  list       A Scheme list.
 * @param  namespace  Active namespace.
 *
 * @return List, or NULL if an element could not be evaluated, or
 *         out of memory.
 */
scheme_pair *scheme_list_evaluated(scheme_pair *list, scheme_namespace *namespace);

#endif
