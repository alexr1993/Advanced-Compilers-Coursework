Advanced Compilers Coursework
=============================

This is a compiler for the --C language (a subset of C) targeting the MIPS instruction set.

Developed entirely in C with a Flex lexer and a Bison parser, it interprets --C and can translate it to an intermediate representation (Three Address Code).

Some headway has been made on MIPS code generation: Register descriptors and basic allocation has been implemented, although instruction selection and other storage problems remain unsolved.
