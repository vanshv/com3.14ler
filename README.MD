# Testing
Compile and start the REPL by running the commands

```console
> g++ obj.cpp lexer.cpp ast.cpp parser.cpp repl.cpp builtins.cpp -o repl
> ./repl
```

You can run the interpreter by testing the code - 
(get rid of the newlines tho)

let fib = fn(x) {
  if (x == 0) {
    0
  } else {
    if (x == 1) {
      return 1;
    } else {
      fib(x - 1) + fib(x - 2);
    }
  }
};

fib(25); //75025