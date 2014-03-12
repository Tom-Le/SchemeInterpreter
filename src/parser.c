#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"

/**** Private function declarations ****/

/**
 * Parse a Scheme expression from specified Scheme file.
 *
 * Will return NULL if we encounter a syntax error.
 *
 * @param  file   A Scheme file.
 * @param  token  Initial token.
 * @param  type   Initial token's type.
 * @param  err    If an error occurs and this is not NULL, it is set
 *                to a value indicating the nature of the error.
 *
 * @return A Scheme element, or NULL if an error occurs.
 */
static scheme_element *_scheme_expression(scheme_file *file, char *token, scheme_token_type type, enum scheme_parser_error *err);

/**
 * Parse a Scheme pair from a Scheme file.
 *
 * Will return NULL if we encounter a syntax error.
 *
 * @param  file   A Scheme file.
 * @param  token  Initial token.
 * @param  type   Initial token's type.
 * @param  err    If an error occurs and this is not NULL, it is set
 *                to a value indicating the nature of the error.
 *
 * @return A Scheme pair, or NULL if an error occurs.
 */
static scheme_element*_scheme_expression_pair(scheme_file *file, char *token, scheme_token_type type, enum scheme_parser_error *err);

/**** Private function implementations ****/

static scheme_element *_scheme_expression(scheme_file *file, char *token, scheme_token_type type, enum scheme_parser_error *err)
{
    if (type == SCHEME_TOKEN_TYPE_LEFT_PARENTHESIS)
    {
        // Get next token and treat as the first element of a Scheme pair.
        scheme_token_type nextType;
        char *nextToken = scheme_get_token(file, &nextType);

        scheme_element *pair = _scheme_expression_pair(file, nextToken, nextType, err);
        free(nextToken);

        return pair;
    }

    if (type == SCHEME_TOKEN_TYPE_SINGLE_QUOTE)
    {
        // Expand '<element> into (quote <element>)
        // Get next token and treat as regular Scheme expression.
        scheme_token_type nextType;
        char *nextToken = scheme_get_token(file, &nextType);

        scheme_element *element = _scheme_expression(file, nextToken, nextType, err);
        free(nextToken);

        // Construct (quote <element>) list.
        scheme_symbol *quoteSymbol = scheme_symbol_new("quote");
        scheme_pair *quotedList = scheme_pair_new(element, (scheme_element *)scheme_pair_get_empty());
        scheme_pair *quoteElem = scheme_pair_new((scheme_element *)quoteSymbol, (scheme_element *)quotedList);

        scheme_element_free(element);
        scheme_element_free((scheme_element *)quoteSymbol);
        scheme_element_free((scheme_element *)quotedList);
        return (scheme_element *)quoteElem;
    }

    if (type == SCHEME_TOKEN_TYPE_EMPTY_LIST)
    {
        return (scheme_element *)scheme_pair_get_empty();
    }

    if (type == SCHEME_TOKEN_TYPE_SYMBOL)
    {
        return (scheme_element *)scheme_symbol_new(token);
    }

    if (type == SCHEME_TOKEN_TYPE_NUMBER)
    {
        // Convert token to a number.
        int value = (int)strtol(token, (char **)NULL, 10);
        return (scheme_element *)scheme_number_new(value);
    }

    if (type == SCHEME_TOKEN_TYPE_TRUE)
    {
        return (scheme_element *)scheme_boolean_get_true();
    }

    if (type == SCHEME_TOKEN_TYPE_FALSE)
    {
        return (scheme_element *)scheme_boolean_get_false();
    }

    // Should not be reachable.
    if (err != NULL) *err = SCHEME_PARSER_ERROR_SYNTAX;
    return NULL;
}

static scheme_element *_scheme_expression_pair(scheme_file *file, char *token, scheme_token_type type, enum scheme_parser_error *err)
{
    // First element of pair can be any Scheme expression.
    scheme_element *first = _scheme_expression(file, token, type, err);
    if (first == NULL)
    {
        // End of file.
        if (err != NULL) *err = SCHEME_PARSER_ERROR_EOF;
        return NULL;
    }

    // Attempt to parse second element.
    scheme_token_type nextType;
    char *nextToken;
    scheme_element *second;
    scheme_pair *pair;

    // Get next token to determine how to construct pair.
    nextToken = scheme_get_token(file, &nextType);
    if (nextToken == NULL)
    {
        // End of file.
        scheme_element_free(first);
        if (err != NULL) *err = SCHEME_PARSER_ERROR_EOF;
        return NULL;
    }

    // If next token is a dot by itself, token following dot should be treated
    // as literal Scheme expression.
    if (strcmp(nextToken, ".") == 0)
    {
        free(nextToken);

        nextToken = scheme_get_token(file, &nextType);
        second = _scheme_expression(file, nextToken, nextType, err);
        free(nextToken);

        // We expect the following token to be a right parenthesis.
        nextToken = scheme_get_token(file, &nextType);
        free(nextToken);
        if (nextType != SCHEME_TOKEN_TYPE_RIGHT_PARENTHESIS)
        {
            // Expectation not met.
            scheme_element_free(first);
            scheme_element_free(second);
            if (err != NULL) *err = SCHEME_PARSER_ERROR_SYNTAX;
            return NULL;
        }
    }

    // ELse, if next token is a right parenthesis, treat second element as empty pair.
    else if (nextType == SCHEME_TOKEN_TYPE_RIGHT_PARENTHESIS)
    {
        free(nextToken);
        second = (scheme_element *)scheme_pair_get_empty();
    }

    // Else, we are reading the next element of a Scheme list.
    // Treat this element as the first element of a nested pair.
    else
    {
        second = (scheme_element *)_scheme_expression_pair(file, nextToken, nextType, err);
        free(nextToken);
    }

    pair = scheme_pair_new(first, second);
    scheme_element_free(first);
    scheme_element_free(second);
    return (scheme_element *)pair;
}

/**** Public functions ****/

scheme_element *scheme_expression(scheme_file *file, enum scheme_parser_error *err)
{
    scheme_token_type type;
    char *token;
    scheme_element *result;

    // Get next token.
    token = scheme_get_token(file, &type);

    if (type == SCHEME_TOKEN_TYPE_NULL)
    {
        // No more token to read.
        *err = SCHEME_PARSER_ERROR_EOF;
        return NULL;
    }

    result = _scheme_expression(file, token, type, err);

    if (token != NULL) free(token);

    return result;
}
