Experimental Scheme interpreter
===============================

This project uses cmake. To compile and install:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ make install

Program will be installed to `/usr/local` by default.

Built-in procedures
-------------------

Basic operations:

    quote
    define
    lambda
    let
    exit

Scheme pair and list operations:

    cons
    car
    cdr
    cadr, caddr, cadddr, caddddr
    last
    append
    list
    length

Predicates:

    equal?
    list?
    symbol?
    number?
    procedure?
    null?

Conditional operations:

    cond
    if
    assoc

Arithmetic operations:

    +
    -
    *

Boolean operations:

    and
    or
    not

Relational operations:

    >
    <
    >=
    <=
    =
