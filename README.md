minus2C
=======

**Advanced Compilers coursework**

* analysis

  * C.flex - Input for the lexical analyser, flex
  * C.tab.c - Parser generated by Bison
  * C.tab.h - Parser header, declares variable used in the lexer
  * C.y - Parser generator input, contains semantic rules
  * lex.yy.c - Lexical scanner (lexer) generated by flex
  * node.c/nodes.h - Defines node struct which composest he parse tree
  * symbol_table.c - For storing and looking up identifiers
  * token.h - For storing and looking up tokens in source

* synthesis
