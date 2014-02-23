/**
 * Scheme expression parser.
 */

#ifndef __SCHEME_PARSER_H__
#define __SCHEME_PARSER_H__

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

#include "lexer.h"
#include "scheme-data-types.h"

/**
 * Parse a Scheme expression from specified Scheme file.
 * Returned element must be freed with scheme_element_free().
 *
 * @param  file  A Scheme file.
 *
 * @return A Scheme element, or NULL if file is empty.
 */
scheme_element *scheme_expression(scheme_file *file);

#endif
