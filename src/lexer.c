#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"

#define SCHEME_BUFFER_MAX_SIZE 1024
#define SCHEME_RETURN_TOKEN_INITIAL_SIZE 64

// Scheme file representation.
struct scheme_file {
    // File pointer.
    FILE *fp;
    // Buffer for reading from file.
    char *buffer;
    // Buffer length.
    int bufferLength;
    // Current position in buffer.
    int bufferPosition;
    // Flag: Was file manually opened?
    int isFileManuallyOpened;
};

/**** Private function definitions ****/

/**
 * Read a line from file onto its buffer.
 * If line is longer than buffer's size, it reads as many characters as the
 * buffer's size permits onto the buffer.
 *
 * @param  file  A Scheme file.
 *
 * @return 1 if new chars were read and stored on buffer,
 *         0 if no new chars were read and stored.
 */
static int _buffer(scheme_file *file);

/**
 * Get next character in file through its buffer.
 * If buffer has been exhausted, it is populated with more characters
 * from file before the next character is returned.
 *
 * @param  file  A Scheme file.
 *
 * @return Next character, or EOF if either there ane no more character to
 *         read, or an I/O error occurred.
 */
static char _next_character(scheme_file *file);

/**** Private function implementations ****/

static int _buffer(scheme_file *file)
{
    FILE *fp = file->fp;
    char newBuffer[SCHEME_BUFFER_MAX_SIZE];

    // If file can no longer be read, halt.
    if (feof(fp) || ferror(fp)) return 0;

    // Copy remainder of buffer onto new buffer.
    int bufferPosition = file->bufferPosition;
    int bufferLength = file->bufferLength;
    int newBufferPosition = 0;
    while (bufferPosition < bufferLength)
        newBuffer[newBufferPosition++] = file->buffer[bufferPosition++];

    // Read from file onto new buffer.
    int availableBufferSize = SCHEME_BUFFER_MAX_SIZE - newBufferPosition;
    if (fgets(newBuffer + newBufferPosition, availableBufferSize, fp) == NULL)
        return 0;

    // Copy from new buffer back onto file's buffer.
    int newBufferLength;
    for (newBufferLength = 0; newBuffer[newBufferLength] != '\0'; ++newBufferLength)
        file->buffer[newBufferLength] = newBuffer[newBufferLength];
    file->buffer[newBufferLength] = '\0';

    file->bufferLength = newBufferLength;
    file->bufferPosition = 0;

    return 1;
}

static char _next_character(scheme_file *file)
{
    if (file->bufferPosition >= file->bufferLength)
        if (_buffer(file) == 0)
            return EOF;

    return file->buffer[file->bufferPosition++];
}

/**** Public function implementations ****/

scheme_file *scheme_open_path(const char *path)
{
    // Open file.
    FILE *fp;
    if ( (fp = fopen(path, "r")) == NULL )
        return NULL;

    // Allocate buffer.
    char *buffer = malloc(sizeof(char) * SCHEME_BUFFER_MAX_SIZE);
    if (buffer == NULL) return NULL;
    *buffer = '\0';

    // Set up scheme_file struct.
    scheme_file *file = malloc(sizeof(scheme_file));
    if (file == NULL) return NULL;
    file->fp = fp;
    file->buffer = buffer;
    file->bufferPosition = 0;
    file->bufferLength = 0;
    file->isFileManuallyOpened = 1;

    return file;
}

scheme_file *scheme_open_file(FILE *fp)
{
    // Allocate buffer for scheme_file.
    char *buffer = malloc(sizeof(char) * SCHEME_BUFFER_MAX_SIZE);
    if (buffer == NULL) return NULL;
    *buffer = '\0';

    // Set up scheme_file struct.
    scheme_file *file = malloc(sizeof(scheme_file));
    if (file == NULL) return NULL;
    file->fp = fp;
    file->buffer = buffer;
    file->bufferPosition = 0;
    file->bufferLength = 0;
    file->isFileManuallyOpened = 0;

    return file;
}

void scheme_close(scheme_file *file)
{
    if (file->isFileManuallyOpened)
        fclose(file->fp);

    free(file->buffer);
    free(file);
}

char *scheme_get_token(scheme_file *file, scheme_token_type *type)
{
    char c;   // Buffer to read file character-by-character.

    // Skip whitespace.
    while (isspace(c = _next_character(file)));

    // Case 0: Nothing to read.
    if (c == EOF)
    {
        if (type != NULL) *type = SCHEME_TOKEN_TYPE_NULL;
        return NULL;
    }

    // Allocate return token.
    // For convenience, this is assumed to be at least 4 chars long.
    char *token = malloc(sizeof(char) * SCHEME_RETURN_TOKEN_INITIAL_SIZE);
    if (token == NULL)
    {
        if (type != NULL) *type = SCHEME_TOKEN_TYPE_NULL;
        return NULL;
    }

    int tokenSize = SCHEME_RETURN_TOKEN_INITIAL_SIZE;

    // Case 1: Right parenthesis.
    if (c == ')')
    {
        strcpy(token, ")");
        if (type != NULL) *type = SCHEME_TOKEN_TYPE_RIGHT_PARENTHESIS;
        return token;
    }

    // Case 2: Quote.
    if (c == '\'')
    {
        strcpy(token, "\'");
        if (type != NULL) *type = SCHEME_TOKEN_TYPE_SINGLE_QUOTE;
        return token;
    }

    // Case 2: Left parenthesis. Could be an empty list.
    if (c == '(')
    {
        // Skip whitespace.
        while (isspace(c = _next_character(file)));

        if (c == ')')
        {
            // Is an empty list.
            strcpy(token, "()");
            if (type != NULL) *type = SCHEME_TOKEN_TYPE_EMPTY_LIST;
            return token;
        }
        else
        {
            // Is only a left parenthesis. Rewind.
            --file->bufferPosition;
            strcpy(token, "(");
            if (type != NULL) *type = SCHEME_TOKEN_TYPE_LEFT_PARENTHESIS;
            return token;
        }
    }

    // Case 3: #t or #f.
    if (c == '#')
    {
        c = _next_character(file);
        if (c == 't')
        {
            strcpy(token, "#t");
            if (type != NULL) *type = SCHEME_TOKEN_TYPE_TRUE;
            return token;
        }
        else if (c == 'f')
        {
            strcpy(token, "#f");
            if (type != NULL) *type = SCHEME_TOKEN_TYPE_FALSE;
            return token;
        }
        else
        {
            // TODO: Support # flags.
            *token = '\0';
            if (type != NULL) *type = SCHEME_TOKEN_TYPE_NULL;
            return token;
        }
    }

    // Case 4: '-', could be by itself, could be a negative number,
    // or a symbol that happens to start with a dash.
    if (c == '-')
    {
        c = _next_character(file);

        if (isspace(c))
        {
            // '-' by itself.
            strcpy(token, "-");
            if (type != NULL) *type = SCHEME_TOKEN_TYPE_SYMBOL;
            return token;
        }
        else
        {
            // Rewind.
            --file->bufferPosition;
            c = '-';
        }
    }

    // Case 5: Symbol or number.
    int isNumber = 1;  // Flag to distinguish numbers from symbols.
    int count = 0;     // Character count.

    // Read into token string till either we encounter a terminal symbol
    // or we exhaust the string's size.
    while (1)
    {
        // Check if token is a number.
        if (isNumber && !isdigit(c))
        {
            // If token's first character is '-', still treat it as a number.
            if (count != 0 || c != '-')
            {
                isNumber = 0;
            }
        }

        token[count] = c;

        c = _next_character(file);
        ++count;

        // Make sure token has enough space.
        if (count == tokenSize)
        {
            int tokenNewSize = tokenSize * 2;
            char *tokenNew = malloc(sizeof(char) * tokenNewSize);
            if (tokenNew == NULL) return NULL;

            memcpy(tokenNew, token, sizeof(char) * tokenSize);

            free(token);

            token = tokenNew;
            tokenSize = tokenNewSize;
        }

        if (c == '(' || c == ')' || c == '\'' || isspace(c) || c == EOF)
        {
            // Encountered terminal symbol.

            // Terminate string.
            token[count] = '\0';
            // Rewind.
            --file->bufferPosition;

            break;
        }
    }

    if (type != NULL)
        *type = isNumber ? SCHEME_TOKEN_TYPE_NUMBER : SCHEME_TOKEN_TYPE_SYMBOL;

    return token;
}
