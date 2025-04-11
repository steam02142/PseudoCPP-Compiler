# PseudoCPP-Compiler
Psuedo is a programming language that aims to follow the [Cal Poly pseudocode standard](https://users.csc.calpoly.edu/~jdalbey/SWE/pdl_std.html), with a few tweaks. PseudoCPP compiles Psuedo into C++.

## Language Features

1. Variable declarations with type inference (for basic datatypes). This includes integers, floats, strings, and booleans.

```
SET var TO 10
SET temp TO 24.3
SET message TO "Hello"
```

2. Functions with basic return types (integers, floats, strings, booleans, and void).

```
FUNCTION printArr ( arrMax: int, arr: real[] ) -> int
    body
ENDFUNCTION
```

3. Array support
```
SET realArr TO [ 2.2, 5.5 ]
SET arrVal TO realArr[0]
```

4. For Loops
```
FOR i FROM 0 TO 10
    body
ENDFOR
```

5. Input/Output with output concatenation for formatting
```
SET varOne TO 0
SET varTwo TO 0

PRINT "Enter value for first number"
READ varOne
PRINT "Enter value for second number"
READ varTwo

PRINT "varOne is: " + varOne + " varTwo is: " + varTwo
```

6. If-else statements, supporting && and || conditions with the following operators: <, <=, >, >=, !
```
IF condition
    body
ELSE
    body
ENDIF 
```

7. Expressions (binary expressions only) supporting the following operators: +, -, /, *, %
```
SET a TO ( 4 / 7)
SET test TO ( 5 + ( 10 % ( a * 4 ) ) )
```

8. Function calls with return types
```
# call add with two parameters and create an addResult 
#   variable to assign the return value to
CALL add WITH a, b RETURNING addResult
```

## Context Sensitive Checks
The compiler throws error messages in the format 'Error (line x, col y): message' and supports the following checks

1. Variables
    - tests for incompatible type assignment (e.g. assigning a string to an integer). This works with expressions as well
    - ensuring variables exist before use

2. Functions
    - ensures actual returned type matches function return type
    - checks number of parameters match function in procedure call
    - checks variable types passed in procedure call match function defintion
    - checks that procedures exists before use


## Building
To build PseudoCPP run
```
make
```

To clean run
```
make clean
```

**Note**: Cleaning will also remove any generated C++ files

## Running Programs
Once you have written your first bit of Psuedo (or grabbed some from the provided samples) do the following:

1. Generate the C++ file 
```
./pseudoCPPx < path_to_Pseudo
```

2. Run make on the generated output file
```
make output
```

3. Run your new program
```
./output
```

When you want to compile another program you can follow these steps again, there is no need to clean in between.

