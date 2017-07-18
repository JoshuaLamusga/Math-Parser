# Math Parser
This is a math parser using the Shunting-Yard algorithm and reverse polish notation to parse math expressions written in C++. It can be generalized to include variables, functions and control flow to create a miniature language. It is cleanly written, though it is not the best implementation because there is unresolved ambiguity between unary and binary minus usage which resulted in using the _ symbol for unary minus.

### Supported syntax
+, -, /, *, ^, and () are supported. Unary minus uses the _ symbol.  
! and % are supported (factorial and modulus).

### Supported functions
**round(n)**: Rounds the given number to the nearest integer.
