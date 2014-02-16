#include "parser.h"

/**** Constants and variables ****/

// Length of longest token.
#define SCHEME_TOKEN_MAX_LENGTH 1024

/**** Private function declarations ****/

/**
 * Parse a Scheme expression from a Scheme file..
 *
 * @param  file   A Scheme file.
 * @param  token  Initial token from file.
 * @param  type   Initial token's type.
 * @param  depth  Expression's depth.
 *
 * @return 1 if expression was parsed, 0 otherwise.
 */
static int _scheme_expression(scheme_file *file, char *token, scheme_token_type type, int depth);

/**** Private function implementations ****/

static int _scheme_expression(scheme_file *file, char *token, scheme_token_type type, int depth)
{
    switch (type)
    {
        case SCHEME_TOKEN_TYPE_LEFT_PARENTHESIS:
            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf("S_Expression\n");
            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf("(\n");

            while (1)
            {
                scheme_token_type nextTokenType;
                char *nextToken = scheme_get_token(file, &nextTokenType);
                _scheme_expression(file, nextToken, nextTokenType, depth + 1);
                if (nextToken != NULL) free(nextToken);

                if (nextTokenType == SCHEME_TOKEN_TYPE_RIGHT_PARENTHESIS)
                    break;
            }

            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf(")\n");

            break;

        case SCHEME_TOKEN_TYPE_RIGHT_PARENTHESIS:
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
            printf("S_Expression (symbol)\n");
            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf("  %s\n", token);

            break;

        case SCHEME_TOKEN_TYPE_NUMBER:
            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf("S_Expression (number)\n");
            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf("  %s\n", token);

            break;

        case SCHEME_TOKEN_TYPE_SINGLE_QUOTE:
            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf("S_Expression\n");
            for (int i = 0; i < depth; ++i, putchar(' '), putchar(' '));
            printf("  '\n");
            break;

        case SCHEME_TOKEN_TYPE_NULL:
            return 0;
            break;
    }

    return 1;
}

/**** Public functions ****/

int scheme_expression(scheme_file *file)
{
    scheme_token_type type;
    char *token = scheme_get_token(file, &type);

    int result = _scheme_expression(file, token, type, 0);

    if (token != NULL) free(token);

    return result;
}
