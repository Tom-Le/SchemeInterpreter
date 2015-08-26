#include <stdio.h>
#include <stdlib.h>

#include "config-info.h"

#include "scheme-data-types.h"
#include "parser.h"
#include "eval.h"
#include "loader.h"
#include "main.h"

#define SCHEME_PROCEDURES_FOLDER "/share/" SCHEME_PROGRAM_NAME "/procedures"

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
    // Parse expressions from stdin until terminated.
    scheme_file *f = scheme_open_file(stdin);

    // Set up procedure loader.
    scheme_loader *loader = scheme_loader_new();
    const char *proceduresPath = SCHEME_INSTALL_PREFIX SCHEME_PROCEDURES_FOLDER;
    int procedureCount = scheme_loader_load_folder(loader, proceduresPath);

    if (procedureCount == 0)
    {
        fprintf(stderr, "WARNING: No built-in procedure found in path '%s'.\n", proceduresPath);
    }

    // Set up base namespace.
    scheme_namespace *baseNamespace = scheme_namespace_new(NULL);
    scheme_loader_put_onto_namespace(loader, baseNamespace);

    printf("Experimental Scheme parser.\n");
    printf("To exit, type \"(exit)\" or the EOF character.\n\n");

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
            {
                putchar('\n');
                break;
            }

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

            if (!scheme_element_is_type(result, scheme_void_get_type()))
            {
                putchar('\n');
            }
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
    scheme_element_free((scheme_element *)baseNamespace);
    scheme_loader_free(loader);
    scheme_close(f);
    return g_SchemeProgramTerminationCode;
}
