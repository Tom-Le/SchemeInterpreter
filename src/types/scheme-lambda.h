/**
 * Scheme lambda procedure.
 *
 * Data type for procedures created by the procedure "lambda".
 *
 * This type is compatible with scheme_procedure.
 * You may cast any scheme_lambda pointer to scheme_procedure and pass it
 * to any function that accepts scheme_procedure.
 */

#ifndef __SCHEME_LAMBDA_H__
#define __SCHEME_LAMBDA_H__

#include "scheme-element.h"

// Scheme lambda procedure type.
typedef struct scheme_lambda scheme_lambda;

// Scheme lambda's argument identifier.
struct scheme_lambda_argument {
    char *id;
    scheme_element *defaultValue;
};

/**
 * Create a new Scheme lambda procedure.
 *
 * @param  name             Optionally specify procedure's name.
 *                          Pass NULL for procedure to be unnamed.
 * @param  arguments        Array of argument identifiers.
 *                          Pass NULL if there is no argument identifier.
 * @param  argumentCount    Count of argument identifiers.
 * @param  restID           Optionally specify a rest ID. Pass NULL if no
 *                          rest ID needed.
 * @param  expressions      Array of Scheme elements to be treated as
 *                          expressions when invoking procedure.
 * @param  expressionCount  Count of expressions.
 *
 * @return Lambda procedure or NULL if out of memory.
 */
scheme_lambda *scheme_lambda_new(char *name,
                                 struct scheme_lambda_argument *arguments,
                                 int argumentCount,
                                 char *restID,
                                 scheme_element **expressions,
                                 int expressionCount);

/**
 * Create a new Scheme lambda procedure from Scheme elements.
 *
 * @param  name         Optionally specify procedure's name.
 *                      Pass NULL for procedure to be unnamed.
 * @param  arguments    A Scheme element containing argument identifiers
 *                      for the procedure.
 * @param  expressions  A Scheme list of expressions to be evaluated
 *                      when procedure is invoked.
 *
 * @return Lambda procedure or NULL if out of memory.
 */
scheme_lambda *scheme_lambda_new_from_elements(char *name,
                                               scheme_element *arguments,
                                               scheme_element *expressions);

/**
 * Get lambda procedure's type.
 *
 * @return Lambda procedure's type.
 */
scheme_element_type *scheme_lambda_get_type();

#endif
