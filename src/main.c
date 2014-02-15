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
    while (1) {
        printf("scheme> ");
        fflush(stdout);
        scheme_expression();
    }

    return 0;
}
