/**
 * Scheme built-in procedure "car".
 * Returns the first element of a Scheme pair.
 */

#ifndef __SCHEME_PROCEDURE_CAR_H__
#define __SCHEME_PROCEDURE_CAR_H__

#include "scheme-procedure.h"

#define PROCEDURE_CAR_NAME "car"

/**
 * Get Scheme procedure "car".
 *
 * @return Scheme procedure "car".
 */
scheme_procedure *scheme_procedure_car();

#endif
