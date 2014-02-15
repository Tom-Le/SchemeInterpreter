#include "parser.h"

/**** Constants and variables ****/
// Length of longest token.
#define SCHEME_MAX_TOKEN_LENGTH 20

// Token types.
typedef enum _scheme_token_type {
    SCHEME_TOKEN_TYPE_OPEN_PARENTHESIS,
    SCHEME_TOKEN_TYPE_CLOSE_PARENTHESIS,
    SCHEME_TOKEN_TYPE_EMPTY_LIST,
    SCHEME_TOKEN_TYPE_TRUE,
    SCHEME_TOKEN_TYPE_FALSE,
    SCHEME_TOKEN_TYPE_SYMBOL
} _scheme_token_type;

/**** Private function declarations ****/

/**
 * Parse a Scheme expression from standard input.
 *
 * @param  token  A token.
 * @param  type   Token's type.
 * @param  depth  Expression's depth.
 */
void _scheme_expression(char *token, _scheme_token_type type, int depth);

/**
 * Get the next token.
 *
 * @param  token  char buffer for storing token's text.
 *
 * @return Token type.
 */
_scheme_token_type _scheme_get_next_token(char **token);

/**** Private function implementations ****/

void _scheme_expression(char *token, _scheme_token_type type, int depth)
{
    switch (type)
    {
        case SCHEME_TOKEN_TYPE_OPEN_PARENTHESIS:
            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf("S_Expression\n");
            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf("(\n");

            _scheme_token_type nextTokenType;
            char *nextToken = malloc(sizeof(char) * SCHEME_MAX_TOKEN_LENGTH);
            do
            {
                nextTokenType = _scheme_get_next_token(&nextToken);
                _scheme_expression(nextToken, nextTokenType, depth + 1);
            }
            while (nextTokenType != SCHEME_TOKEN_TYPE_CLOSE_PARENTHESIS);

            free(nextToken);

            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf(")\n");

            break;

        case SCHEME_TOKEN_TYPE_CLOSE_PARENTHESIS:
            break;

        case SCHEME_TOKEN_TYPE_EMPTY_LIST:
            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf("S_Expression\n");
            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf("  ()\n");

            break;

        case SCHEME_TOKEN_TYPE_TRUE:
            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf("S_Expression\n");
            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf("  #t\n");

            break;

        case SCHEME_TOKEN_TYPE_FALSE:
            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf("S_Expression\n");
            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf("  #f\n");

            break;

        case SCHEME_TOKEN_TYPE_SYMBOL:
            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf("S_Expression\n");
            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf("  %s\n", token);

            break;
    }
}

_scheme_token_type _scheme_get_next_token(char **token)
{
    // Copy token.
    strcpy(*token, getToken());

    // Determine type.
    if (strcmp(*token, "(") == 0)
        return SCHEME_TOKEN_TYPE_OPEN_PARENTHESIS;
    else if (strcmp(*token, ")") == 0)
        return SCHEME_TOKEN_TYPE_CLOSE_PARENTHESIS;
    else if (strcmp(*token, "()") == 0)
        return SCHEME_TOKEN_TYPE_EMPTY_LIST;
    else if (strcmp(*token, "#t") == 0)
        return SCHEME_TOKEN_TYPE_TRUE;
    else if (strcmp(*token, "#f") == 0)
        return SCHEME_TOKEN_TYPE_FALSE;
    else
        return SCHEME_TOKEN_TYPE_SYMBOL;
}

/**** Public functions ****/

void scheme_expression(void)
{
    // Get first token and treat as an expression.
    startTokens(SCHEME_MAX_TOKEN_LENGTH);

    char *token = malloc(sizeof(char) * SCHEME_MAX_TOKEN_LENGTH);
    _scheme_token_type type = _scheme_get_next_token(&token);
    _scheme_expression(token, type, 0);
    free(token);
}
