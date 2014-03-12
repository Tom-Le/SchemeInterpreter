/**
 * Scheme expression evaluator.
 */

#ifndef __SCHEME_EVALUATOR_H__
#define __SCHEME_EVALUATOR_H__

#include "scheme-data-types.h"

/**
 * Evaluate a Scheme element.
 * Returned element must be freed with scheme_element_free().
 *
 * @param  element    Element to be evaluated.
 * @param  namespace  A Scheme namespace. Symbols found during evaluation will
 *                    be resolved using this namespace.
 *
 * @return Result, or NULL if expression could not be evaluated.
 */
scheme_element *scheme_evaluate(scheme_element *element, scheme_namespace *namespace);

#endif
