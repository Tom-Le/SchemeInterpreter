/**
 * Scheme lexical analyzer.
 */

#ifndef __SCHEME_LEXICAL_ANALYZER_H__
#define __SCHEME_LEXICAL_ANALYZER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "strutil.h"

// Scheme file.
typedef struct scheme_file scheme_file;

// Token types.
typedef enum scheme_token_type {
    // Left parenthesis.
    SCHEME_TOKEN_TYPE_LEFT_PARENTHESIS,
    // Right parenthesis.
    SCHEME_TOKEN_TYPE_RIGHT_PARENTHESIS,
    // Empty list.
    SCHEME_TOKEN_TYPE_EMPTY_LIST,
    // Single quote.
    SCHEME_TOKEN_TYPE_SINGLE_QUOTE,
    // True (#t)
    SCHEME_TOKEN_TYPE_TRUE,
    // False (#f)
    SCHEME_TOKEN_TYPE_FALSE,
    // Symbol (arbitrary text)
    SCHEME_TOKEN_TYPE_SYMBOL,
    // Number
    SCHEME_TOKEN_TYPE_NUMBER,
    // File cannot be read any further.
    SCHEME_TOKEN_TYPE_NULL
} scheme_token_type;

/**
 * Open a new Scheme file at specified path.
 *
 * @param  path  Path to file to be opened.
 *
 * @return Scheme file, or NULL if the path could not be opened,
 *     or if out of memory.
 */
scheme_file *scheme_open_path(const char *path);

/**
 * Open a new Scheme file from file pointer.
 * Caller is responsible for closing file pointer itself.
 *
 * @param  fp  File pointer.
 *
 * @return Scheme file, or NULL if out of memory.
 */
scheme_file *scheme_open_file(FILE *fp);

/**
 * Close Scheme file.
 * If file was previously opened with scheme_open_file(), the
 * caller is responsible for closing the file pointer supplied to
 * scheme_open_file().
 *
 * @param  file  File to be closed.
 */
void scheme_close(scheme_file *file);

/**
 * Get next available token.
 * This token must be freed with free() afterwards.
 *
 * @param  file  A Scheme file.
 * @param  type  If supplied, set to token's type.
 *
 * @return String containing token, or NULL if there is no more token.
 */
char *scheme_get_token(scheme_file *file, scheme_token_type *type);

#endif
