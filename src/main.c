#include <stdio.h>
#include <stdlib.h>

#include "scheme-data-types.h"
#include "parser.h"
#include "eval.h"
#include "main.h"

int g_SchemeProgramTerminationFlag = 0;
int g_SchemeProgramTerminationCode = 0;

/**
 * Start an interactive Scheme prompt.
 *
 * @param  argc  Argument count.
 * @param  argv  Arguments.
 */
int main(int argc, char *argv[])
{
    printf("Experimental Scheme parser.\n");
    printf("To exit, type \"(exit)\" or the EOF character.\n\n");

    // Parse expressions from stdin until terminated.
    scheme_file *f = scheme_open_file(stdin);

    // Set up base namespace.
    scheme_namespace *baseNamespace = scheme_namespace_base_new(NULL);

    while (1)
    {
        printf("> ");
        fflush(stdout);

        // Read an expression.
        enum scheme_parser_error parserError;
        scheme_element *expression = scheme_expression(f, &parserError);

        if (expression == NULL)
        {
            // End of file.
            if (parserError == SCHEME_PARSER_ERROR_EOF)
                break;

            // Syntax error.
            printf("Syntax error.\n");
            continue;
        }

        // Evaluate expression.
        scheme_element *result = scheme_evaluate(expression, baseNamespace);
        if (result == NULL)
        {
            printf("Could not evaluate: ");
            scheme_element_print(expression);
            putchar('\n');
        }
        else
        {
            // Print evaluated result.
            scheme_element_print(result);
            putchar('\n');
        }

        scheme_element_free(expression);
        scheme_element_free(result);

        // Check termination flag.
        if (g_SchemeProgramTerminationFlag)
        {
            break;
        }
    }

    // Terminate.
    printf("\n");
    scheme_element_free((scheme_element *)baseNamespace);
    scheme_close(f);
    return g_SchemeProgramTerminationCode;
}
