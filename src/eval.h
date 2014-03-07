/**
 * Scheme expression evaluator.
 */

#include "scheme-data-types.h"

/**
 * Evaluate a Scheme element.
 * Returned element must be freed with scheme_element_free().
 *
 * @param  element  Element to be evaluated.
 *
 * @return Result of evaluation, or NULL if element could not
 *     be evaluated.
 */
scheme_element *scheme_evaluate(scheme_element *element);
