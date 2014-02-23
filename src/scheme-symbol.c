#include <stdio.h>
#include <stdlib.h>

#include "scheme-symbol.h"
#include "scheme-element-private.h"

#define SCHEME_SYMBOL_INITIAL_LENGTH 1024

// Scheme identifier symbol.
struct scheme_symbol {
    struct scheme_element super;
    char *identifier;
    int length;  // Length of identifier NOT including \0
};

/**** Private function declarations ****/

/**
 * Return symbol's type identifier.
 */
static char *_scheme_symbol_get_type(scheme_element *element);

/**
 * Free Scheme symbol.
 *
 * @param  element  Should be a Scheme symbol.
 */
static void _scheme_symbol_free(scheme_element *element);

/**
 * Print symbol to stdout.
 */
static void _scheme_symbol_print(scheme_element *element);

/**** Private variables ****/

// Global virtual function table.
static struct scheme_element_vtable _scheme_symbol_vtable = {
    _scheme_symbol_get_type,
    _scheme_symbol_free,
    _scheme_symbol_print
};

/**** Private function implementations ****/

static char *_scheme_symbol_get_type(scheme_element *element)
{
    return SCHEME_SYMBOL_TYPE;
}

void _scheme_symbol_free(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_SYMBOL_TYPE))
        return;

    scheme_symbol *symbol = (scheme_symbol *)element;

    free((scheme_symbol *)symbol->identifier);
    free(symbol);
}

static void _scheme_symbol_print(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_SYMBOL_TYPE))
        return;

    scheme_symbol *symbol = (scheme_symbol *)element;
    printf("%s", symbol->identifier);
}

/**** Public function implementations ****/

scheme_symbol *scheme_symbol_new(char *identifier)
{
    // Allocate symbol.
    scheme_symbol *symbol;
    if ((symbol = malloc(sizeof(scheme_symbol))) == NULL)
        return NULL;

    // Set up virtual function table.
    symbol->super.vtable = &_scheme_symbol_vtable;

    // Allocate identifier string.
    char *idBuffer;
    int bufLen = SCHEME_SYMBOL_INITIAL_LENGTH;
    if ((idBuffer = malloc(sizeof(char) * bufLen)) == NULL)
        return NULL;

    // Copy identifier.
    int i = 0;
    while (identifier[i] != '\0')
    {
        // Make sure buffer has enough space.
        if ((i-1) == bufLen)
        {
            int bufLenNew = bufLen * 2;

            char *idBufferNew = reallocate_char(idBuffer, bufLen, bufLenNew);

            free(idBuffer);

            idBuffer = idBufferNew;
            bufLen = bufLenNew;
        }

        idBuffer[i] = identifier[i];

        ++i;
    }
    idBuffer[i] = '\0';

    symbol->identifier = idBuffer;
    symbol->length = i;

    return symbol;
}

char *scheme_symbol_get_value(scheme_symbol *symbol)
{
    // Allocate return buffer.
    char *returnBuf;
    int length = symbol->length + 1;  // Make room for \0
    if ((returnBuf = malloc(sizeof(char) * length)) == NULL)
        return NULL;

    // Copy identifier.
    char *identifier = symbol->identifier;
    int i;
    for (i = 0; i < length && identifier[i] != '\0'; ++i)
    {
        returnBuf[i] = identifier[i];
    }
    returnBuf[i] = '\0';

    return returnBuf;
}
