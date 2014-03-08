/**
 * Utilities for Scheme procedures.
 */

#ifndef __SCHEME_PROCEDURE_UTILS_H__
#define __SCHEME_PROCEDURE_UTILS_H__

#include "scheme-data-types.h"

/**
 * Get arguments passed to a Scheme procedure.
 *
 * Will return NULL if supplied element is not a pair in the format:
 *   (<arg 1> <arg 2> <arg 3> ... <arg n>)
 * In this case count, if not NULL, is set to -1.
 *
 * Will also return NULL if supplied element is the empty pair, ie.
 * there are no arguments. In this case count, if not NULL, is set
 * to 0.
 *
 * Caller is responsible for freeing returned pointer with free().
 *
 * @param  element  A Scheme element containing arguments.
 * @param  count    If not NULL, this is set to the number of arguments.
 *
 * @return A list of arguments or NULL if element is not in the
 *   format listed above.
 */
scheme_element **procedure_get_arguments(scheme_element *element, int *count);

#endif
