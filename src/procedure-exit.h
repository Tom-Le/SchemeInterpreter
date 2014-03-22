/**
 * Scheme built-in procedure "exit".
 *
 * Signal main program to terminate. If given a number, set exit code
 * to given number.
 */

#ifndef __SCHEME_PROCEDURE_EXIT_H__
#define __SCHEME_PROCEDURE_EXIT_H__

#include "scheme-procedure.h"

#define PROCEDURE_EXIT_NAME "exit"

/**
 * Get Scheme procedure "exit".
 */
scheme_procedure *scheme_procedure_exit();

#endif
