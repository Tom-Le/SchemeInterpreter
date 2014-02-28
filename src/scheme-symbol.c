#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * Initialize static variables.
 */
static void _static_init();

/**
 * Return symbol's type identifier.
 */
static char *_vtable_get_type(scheme_element *element);

/**
 * Free Scheme symbol.
 *
 * @param  element  Should be a Scheme symbol.
 */
static void _vtable_free(scheme_element *element);

/**
 * Print symbol to stdout.
 */
static void _vtable_print(scheme_element *element);

/**
 * Copy symbol.
 */
static scheme_element *_vtable_copy(scheme_element *element);

/**** Private variables ****/

// Global virtual function table.
static struct scheme_element_vtable _scheme_symbol_vtable;

static int _static_initialized = 0;

/**** Private function implementations ****/

static void _static_init()
{
    if (!_static_initialized)
    {
        _scheme_symbol_vtable.get_type = _vtable_get_type;
        _scheme_symbol_vtable.free = _vtable_free;
        _scheme_symbol_vtable.print = _vtable_print;
        _scheme_symbol_vtable.copy = _vtable_copy;

        _static_initialized = 1;
    }
}

static char *_vtable_get_type(scheme_element *element)
{
    return SCHEME_SYMBOL_TYPE;
}

void _vtable_free(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_SYMBOL_TYPE))
        return;

    scheme_symbol *symbol = (scheme_symbol *)element;

    free((scheme_symbol *)symbol->identifier);
    free(symbol);
}

static void _vtable_print(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_SYMBOL_TYPE))
        return;

    scheme_symbol *symbol = (scheme_symbol *)element;
    printf("%s", symbol->identifier);
}

static scheme_element *_vtable_copy(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_SYMBOL_TYPE))
        return NULL;

    scheme_symbol *symbol = (scheme_symbol *)element;
    return (scheme_element *)scheme_symbol_new(symbol->identifier);
}

/**** Public function implementations ****/

scheme_symbol *scheme_symbol_new(char *identifier)
{
    _static_init();

    // Allocate symbol.
    scheme_symbol *symbol;
    if ((symbol = malloc(sizeof(scheme_symbol))) == NULL)
        return NULL;

    // Set up virtual function table.
    symbol->super.vtable = &_scheme_symbol_vtable;

    // Copy identifier string.
    char *idBuffer;
    int bufLen = strlen(identifier) + 1;  // Make space for \0
    if ((idBuffer = malloc(sizeof(char) * bufLen)) == NULL)
    {
        free(symbol);
        return NULL;
    }

    strcpy(idBuffer, identifier);

    symbol->identifier = idBuffer;
    symbol->length = bufLen;

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