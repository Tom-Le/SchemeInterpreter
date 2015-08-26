/**
 * Scheme built-in procedure "define".
 *
 * 1. Associate an identifier with a Scheme element in current namespace.
 *    For this usage, it has the following format:
 *          (define <identifier> <element>)
 *
 * 2. Define a lambda procedure.
 *    For this usage, it has the following format:
 *          (define (<procedure name> <arguments> ...) <expressions> ...)
 */

#ifndef __SCHEME_PROCEDURE_DEFINE_H__
#define __SCHEME_PROCEDURE_DEFINE_H__

#include "scheme-procedure.h"

#define PROCEDURE_DEFINE_NAME "define"

/**
 * Get Scheme procedure "define".
 *
 * @return Scheme procedure "define".
 */
scheme_procedure *scheme_procedure_get();

#endif
