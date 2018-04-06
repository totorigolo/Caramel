# Semantic Errors

Undefined symbol error (variables, functions and arrays):
```
semantic error at 10:12: The variable 'y' is not defined before.
10:12    int32_t x = y;
```

Multi-declaration error for variables (multiple declaration for function are ignored if they match)
```
semantic error at 16:4: Cannot use identifier: 'a' because a variable with the same name is already declared at line 15
16:4    extern int32_t a;
        ~~~~~~~~~~~~~~~~
```

Mutli-definition error for symbols (variables, arrays and functions)
```
semantic error at 22:0: Cannot use identifier: 'b' because a variable with the same name is already declared at line 21
22:0    extern int64_t b;
        ~~~~~~~~~~~~~~~~
semantic error at 41:0: foo2 previously declared as a variable, so it's impossible to redeclare it as a function.
41:0    void foo2(char g);
        ~~~~~~~~~~~~~~~~~
semantic error at 45:0: bar2 previously declared as an array, so it's impossible to redeclare it as a function.
45:0    void bar2(char g);
        ~~~~~~~~~~~~~~~~~
```

Array pointer assignment (we cannot assign a value to the array pointer)
```
semantic error at 30:0: Cannot define the variable'd' which has a previous occurrence as a ArraySymbol
30:0    d = 2;
        ~
```

Mismatch between function declaration and function definition (arguments type and arugments count)
If the name of the arguments are not the same, we will put a `WARNING`.
```
semantic error at 34:0: The function: foo was previously declared with 2 parameter(s).
Actual definition has 1 parameter(s).
34:0    void foo(int8_t f)
        ~~~~~~~~~~~~~~~~~~
[WARNING] 49:0: foo3's parameter name j mismatches with the previously declared parameter i.
49:0    void foo3(int8_t h, char j)
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~
semantic error at 56:0: The function: 'bar3' was previously declared with a parameter of type char .
Parameter type found is int8_t.
56:0    void bar3(int8_t k, int8_t l)
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
```

We also put warning when we detect unused variables.
```
[WARNING] The element 'b' was declared but never defined
```
