/**
 * Scheme expression parser.
 */

#ifndef __SCHEME_PARSER_H__
#define __SCHEME_PARSER_H__

#include <stdio.h>
#include <strings.h>
#include "lexer.h"

/**
 * Parse a Scheme expression from a specified Scheme file.
 *
 * @param  file  A Scheme file.
 *
 * @return 1 if expression was parsed, 0 otherwise.
 */
int scheme_expression(scheme_file *file);

#endif
