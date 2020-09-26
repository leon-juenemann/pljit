# pljit
just-in-time compiler for a simple programming language

Final project from TUM course: "Practical Course: Systems Programming in C++"

• A user of the library can register a number of functions by passing their source code to
  the library.
• The library exposes an interface that allows its users to call these registered functions.
• Once a function is called for the first time, the library compiles the source code string into
  an optimized abstract syntax tree that can be evaluated to obtain the result of a function
  call.
