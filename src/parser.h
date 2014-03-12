/**
 * Scheme expression parser.
 */

#ifndef __SCHEME_PARSER_H__
#define __SCHEME_PARSER_H__

#include "lexer.h"
#include "scheme-data-types.h"

// Possible error codes.
enum scheme_parser_error {
    SCHEME_PARSER_ERROR_SYNTAX,
    SCHEME_PARSER_ERROR_EOF
};

/**
 * Parse a Scheme expression from specified Scheme file.
 * Returned element must be freed with scheme_element_free().
 *
 * Will return NULL if if file has been exhausted or if we encounter a
 * syntax error.
 *
 * @param  file  A Scheme file.
 * @param  err   If an error occurs and this is not NULL, it is set to
 *               a value indicating the nature of the errer.
 *
 * @return A Scheme element, or NULL if an error occurs.
 */
scheme_element *scheme_expression(scheme_file *file, enum scheme_parser_error *err);

#endif
