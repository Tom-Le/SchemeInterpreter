#include "parser.h"

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
    while (1)
    {
        printf("scheme> ");
        fflush(stdout);
        if (scheme_expression(f) == 0)
            break;
    }
    scheme_close(f);

    return 0;
}
