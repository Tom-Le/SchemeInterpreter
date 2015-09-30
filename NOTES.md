Notes on program structure
--------------------------

There are 5 main modules:

   - The main program in `main.h` and `main.c`.
   - The procedure loader in `loader.h` and `loader.c`.
   - The lexical analyzer in `lexer.h` and `lexer.c`.
   - The Scheme expression parser in `parser.h` and `parser.c`.
   - The Scheme expression evaluator in `eval.h` and `eval.c`.

There are 5 fundamental Scheme data types:

   - Scheme element: An abstract type, represents any Scheme element. Implemented in
     `scheme-element.h`, `scheme-element-private.h`, and `scheme-element.c`.
   - Number: Represents an integer. Implemented in `scheme-number.h` and `scheme-number.c`.
   - Boolean symbol: Represents a boolean symbol (#t or #f). Implemented in `scheme-boolean.h` and
     `scheme-boolean.c`.
   - Symbol: Represents a generic string of characters. Implemented in `scheme-symbol.h` and
     `scheme-symbol.c`.
   - Pair: Represents a pair of two elements. Implemented in `scheme-pair.h` and `scheme-pair.c`.
     Note that a Scheme list is defined to be either the empty pair or a pair whose second element
     is also a list.

In addition, there are 4 more Scheme data types:

   - Procedure: Represents a procedure, which contains a C function that can evaluate a Scheme
     element and return the result. Implemented in `scheme-procedure.h`, `scheme-procedure-init.h`
     and `scheme-procedure.c`.
   - Lambda procedure: Represents a procedure created by the procedure `lambda`. It contains a list
     of argument identifiers, a list of expressions, and a C function that assigns values from a
     given Scheme pair to the identifiers then evaluate the expressions sequentially, returning the
     result of the last evaluation. It is defined in `scheme-lambda.h` and `scheme-lambda.c`.
   - Namespace: Maps symbols to Scheme elements. Implemented in `scheme-namespace.h` and
     `scheme-namespace.c`. The main program sets up a base namespace containing built-in procedures
     before parsing expressions from standard input.
   - Void symbol: Represents no output. Implemented in `scheme-void.h` and `scheme-void.c`. I
     currently don't use it anywhere.

All built-in procedures exist as semi-standalone dynamic libraries, configured to install to a
specific location that is checked by main program upon startup.

Notes on implementations
------------------------

### Main program

The main program initializes a base namespace that contains every built-in Scheme procedure. It then
reads Scheme expressions and evaluates them until either it receives the EOF character, or the user
invokes the procedure `exit`.

### Lexical analyzer

The lexical analyzer takes a Scheme file as the input and returns the next token in the file. It
reads files in a linear fashion and does not allow seeking back and re-reading tokens that have
already been read.

When asked to read a token from a file, it reads the first character that is being pointed at in the
file using its internal version of `getchar()`, named `_next_character()`, then use that character
to determine the token's type.  There are 8 token types:

   - Left parenthesis
   - Right parenthesis.
   - Empty list (a left parenthesis, followed by an optional string of whitespace characters, then a
     right parenthesis).
   - Single quote.
   - The true symbol #t.
   - The false symbol #f.
   - Symbol (an arbitrary string).
   - Number, including negative numbers.

In addition, the lexical analyzer will return a null token if the given file has been completely
exhausted.

Internally, a Scheme file is accompanied by a buffer that can hold at most 1024 characters. This has
a couple of benefits:

   - The lexical analyzer handles lines of arbitrary lengths. If it finds a line that is longer than
     1023 characters, it stores the first 1023 characters in the buffer, then fills the buffer with
     the next 1023 characters after the buffer is exhausted, and so on. Note that if a line has less
     than 1023 characters, it does not attempt to read more lines until the buffer becomes full.
   - The lexical analyzer can peek at the next character and wind back one character if necessary.
     It can only rewind at most one character at a time. I found it unnecessary to rewind multiple
     characters at once.

The lexical analyzer also provides routines for opening and closing a Scheme file. The first routine
opens a file pointer and initializes its accompanying buffer. The second one deallocates those
resources.

### Parser

Using the lexical analyzer, the parser reads the next Scheme expression in a Scheme file.
Internally, the function `scheme_expression()` is a wrapper for `_scheme_expression()`. This routine
handles tokens of every type except for pairs. For simple tokens, specifically for numbers, symbols,
true/false symbols, and empty lists, it simply constructs and returns a Scheme element of the
appropriate type.

When it finds a single quote, it recursively read the next Scheme expression, then constructs and
returns a Scheme expression in the form of "(quote <element>)" where "<element>" is the expression
we recursively read.

When it finds a left parenthesis, it delegates to another routine, `_scheme_expression_pair()`. I
found this necessary in order to process different forms of writing Scheme pairs and lists. In
particular, it supports the following forms:

   - Lists, e.g. "(1 2 3 4 5)".
   - Pairs with exactly two elements, e.g. "(1 . 2)".
   - Other non-list pairs, e.g. "(1 2 3 . 4)".

Internally, every list is treated as either the empty pair or a pair whose second element is also a
list. For example, (1 2 3) is equivalent to (1 . (2 . (3 . ()))).

The parser also returns NULL if either the expression contains a syntax error or the file has been
exhausted.

### Evaluator

The evaluator has two responsibilities: Resolve symbols and execute other Scheme expressions. When
it finds a symbol, it uses the given namespace (implemented in `scheme-namespace.h` and
`scheme-namespace.c`) to resolve the symbol to a Scheme element. The namespace will either return
the matching Scheme element or NULL if no such element exists.

WHen it finds a pair, it first recursively evaluates the first element of the pair. This has to
evaluate to a Scheme procedure, either built-in or user-defined, or it returns NULL. It then passes
the second element of the pair, which is left unevaluated, to the procedure using
`scheme_procedure_apply()` defined in `scheme-procedure.c`.

### Procedures

A procedure, whether built-in or user-defined, contains a C function that processes the Scheme
element received from the evaluator. It will evaluate this element using the evaluator if necessary.

A lambda (user-defined) procedure stores a list of expressions and a list of argument names. The
procedure's C function treats the element given from the evaluator as a list of arguments, assigns
elements in that list to argument names using a local namespace, then evaluates expressions stored
in the procedure in order. It returns the result of the last expression.

