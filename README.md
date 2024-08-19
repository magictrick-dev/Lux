# Lux Programming Language Compiler

A compiler for the Lux programming language.

# About

Lux is an exploratory programming language designed to behave a lot like C. Lux
provides explicit type-sizing and an intuitive import system to make compiling
easier. Below, you will find an example of Lux in action:

```
import console;

fibonacci(n: i32): i32
{

    if (n <= 1) return(n);
    result := fibonacci(n - 1) + fibonacci(n - 2);
    return(result);

}

main(argc: i32, argv: c8*): i32
{
    
    if (argc < 2)
    {
        console::printf("Useage: Supply a number to calculate the fibonacci value.\n");
        return(1);
    }

    start := stoi32(argv[1]);
    result := fibonacci(start);

    console::printf("Fibonacci result %i\n", result);

    return(0);

}
```

# Documentation

This project is still actively being worked on and therefore documentation is
fairly limited. As this project is iterated on, more documentation will be added.

# License

Copyright (c) 2024 Christopher DeJong / magictrick-dev on GitHub

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
