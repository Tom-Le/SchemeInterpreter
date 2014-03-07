#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "eval.h"

/**
 * Start an interactive Scheme prompt.
 *
 * @param  argc  Argument count.
 * @param  argv  Arguments.
 */
int main(int argc, char *argv[])
{
    printf("Experimental Scheme parser.\n");

    // Parse expressions from stdin until terminated.
    scheme_file *f = scheme_open_file(stdin);
    while (1)
    {
        printf("> ");
        fflush(stdout);

        // Read an expression.
        scheme_element *element = scheme_expression(f);
        if (element == NULL) break;

        // Evaluate expression.
        scheme_element *result = scheme_evaluate(element);
        if (result != NULL)
        {
            // Print evaluated result.
            scheme_element_print(result);
        }
        else
        {
            // We were not able to evaluate expression.
            // Print original expresison instead.
            scheme_element_print(element);
        }

        printf("\n");

        scheme_element_free(element);
        scheme_element_free(result);
    }

    // Terminate.
    printf("\n");
    scheme_close(f);
    return 0;
}
