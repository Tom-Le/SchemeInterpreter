#include "parser.h"

/**** Private function declarations ****/

/**
 * Parse a Scheme expression from specified Scheme file.
 *
 * @param  file   A Scheme file.
 * @param  token  Initial token.
 * @param  type   Initial token's type.
 *
 * @return A Scheme element, or NULL if file is empty.
 */
static scheme_element *_scheme_expression(scheme_file *file, char *token, scheme_token_type type);

/**
 * Parse a Scheme pair from a Scheme file.
 *
 * Will return NULL if file is empty or has been exhausted,
 * or if syntax is invalid.
 *
 * @param  file   A Scheme file.
 * @param  token  Initial token.
 * @param  type   Initial token's type.
 *
 * @return A Scheme pair, or NULL if no pair was parsed.
 */
static scheme_pair *_scheme_expression_pair(scheme_file *file, char *token, scheme_token_type type);

/**** Private function implementations ****/

static scheme_element *_scheme_expression(scheme_file *file, char *token, scheme_token_type type)
{
    if (type == SCHEME_TOKEN_TYPE_NULL)
    {
        return NULL;
    }

    if (type == SCHEME_TOKEN_TYPE_LEFT_PARENTHESIS)
    {
        // Get next token and treat as the first element of a Scheme pair.
        scheme_token_type nextType;
        char *nextToken = scheme_get_token(file, &nextType);

        scheme_pair *pair = _scheme_expression_pair(file, nextToken, nextType);
        free(nextToken);

        return (scheme_element *)pair;
    }

    if (type == SCHEME_TOKEN_TYPE_SINGLE_QUOTE)
    {
        // Expand '<element> into (quote <element>)
        // Get next token and treat as regular Scheme expression.
        scheme_token_type nextType;
        char *nextToken = scheme_get_token(file, &nextType);

        scheme_element *element = _scheme_expression(file, nextToken, nextType);
        free(nextToken);

        // Construct (quote <element>) list.
        scheme_symbol *quoteSymbol = scheme_symbol_new("quote");
        scheme_pair *quoteList = scheme_pair_new(element, (scheme_element *)scheme_pair_get_empty());

        return (scheme_element *)scheme_pair_new((scheme_element *)quoteSymbol, (scheme_element *)quoteList);
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
    return NULL;
}

static scheme_pair *_scheme_expression_pair(scheme_file *file, char *token, scheme_token_type type)
{
    // First element of pair can be any Scheme expression.
    scheme_element *first = _scheme_expression(file, token, type);
    if (first == NULL)
    {
        // End of file.
        return NULL;
    }

    // Attempt to parse second element.
    scheme_token_type nextType;
    char *nextToken;
    scheme_element *second;

    // Get next token to determine how to construct pair.
    nextToken = scheme_get_token(file, &nextType);
    if (nextToken == NULL)
    {
        // End of file.
        scheme_element_free(first);
        return NULL;
    }

    // If next token is a dot by itself, token following dot should be treated
    // as literal Scheme expression.
    if (strcmp(nextToken, ".") == 0)
    {
        free(nextToken);
        nextToken = scheme_get_token(file, &nextType);
        second = _scheme_expression(file, nextToken, nextType);

        // We expect the following token to be a right parenthesis.
        free(nextToken);
        nextToken = scheme_get_token(file, &nextType);
        free(nextToken);
        if (nextType != SCHEME_TOKEN_TYPE_RIGHT_PARENTHESIS)
        {
            // Expectation not met.
            scheme_element_free(first);
            scheme_element_free(second);
            return NULL;
        }

        return scheme_pair_new(first, second);
    }

    // If next token is a right parenthesis, treat second element as empty pair.
    if (nextType == SCHEME_TOKEN_TYPE_RIGHT_PARENTHESIS)
    {
        free(nextToken);
        second = (scheme_element *)scheme_pair_get_empty();
        return scheme_pair_new(first, second);
    }

    // Else, we are reading the next element of a Scheme list.
    // Treat this element as the first element of a nested pair.
    second = (scheme_element *)_scheme_expression_pair(file, nextToken, nextType);
    free(nextToken);

    return scheme_pair_new(first, second);
}

/**** Public functions ****/

scheme_element *scheme_expression(scheme_file *file)
{
    scheme_token_type type;
    char *token;
    scheme_element *result;

    token = scheme_get_token(file, &type);
    result = _scheme_expression(file, token, type);

    if (token != NULL) free(token);
    return result;
}
