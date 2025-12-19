# fltt-compiler
Compiler project for the Formal Languages and Translation Techniques course in the 5th semester at Wrocław University of Science and Technology


# Project structure
`src/` - compiler source code  
`mw2025` - virutal machine by [prof Maciej Gębala](https://cs.pwr.edu.pl/gebala/)  


# Language Structure

The structure is described by the fltt-compiler's language, some parameters are encapsulated in `< >` brackets

## main()
```c++
PROGRAM IS
    // declarations
IN
    // code ...
END
```

## functions
### body
```c++
PROCEDURE name(<F> arg1, <F> arg2, ...) IS
    // declarations
IN
    // function body
END
```

Where **`F`** is an argument type:
 - **`I`** - aka `readonly const`
 - **`O`** - unassigned output arg
 - **`T`** - array
 - $\epsilon$ - no arg type 

technically this should be called a procedure, but following the standard c-like naming convention, I'll using `function` instead of `procedure` do describe procedures in the documentation

### calls
```
name(arg1, arg2, ...);
```

## if statement
```c++
IF <condition> THEN
    // code ...
ENDIF
```
```c++
IF <condition> THEN
    // code ...
ELSE
    // code ...
ENDIF
```

## loops
### for loop
```c++
FOR <identifier> FROM <val> TO <val> DO
    // code ...
ENDFOR
```

### while loop
```c++
WHILE <condition> DO
     // code ...
ENDWHILE
```

### do-while loop
```c++
REPEAT
    // code ...
UNTIL <condition>;
```

## I/O
```
READ <identifier>
WRITE <val>
```



# Rules
## 1. All arithmetic operations are done on natural numbers:
   - $x, y \in [0..\infin)$
   - $x<0 \rarr x=0$ 
   - $\frac{x}{0}=0,\ rem=0$  
```
example:
2 - 3 = 0
2 / 3 = 0
2 / 0 = 0
2 % 0 = 0
```

## 2. `arr[a:b]` declares an array of indicies $[a, a+1, ..., b]$ (inclusive)
```
example: 
arr[3, 7] = array of size 5, ind: [3, 4, 5, 6, 7]
```

## 3. No recursion

## 4. Declarations
All variables must be declared inside a procedure, or be passed as `IN/OUT` ref arguments
 - **`I`** (IN) param marks an argument as `readonly const`
 - **`O`** (OUT) param marks an argumenta as an 'OUT' argument - cannot be read before assignment, cannot be passed as `I`
 - **`T`** marks an argument as an array

## 5. for loops
FOR loop has a non-modifiable local iterator which can only be incremented by $\plusmn1$

## 6. Identifier syntax
Identifier is described with regular expression `[_a-z]+`

## 7. Numerical values
All numbers
 - are 64-bit unsigned integers in the code
 - have no numerical upper limit for every operation


# Virtual Machine
There are 8 registers available, each named

> **[RA]**, 
> **[RB]**, 
> **[RC]**, 
> **[RD]**, 
> **[RE]**, 
> **[RF]**, 
> **[RG]**, 
> **[RH]**, 

let **`PC`** be our program counter
let `MEM[i]` be the memory at address *`i`*

|instruction|arg|action|PC|cost|description|
|:---|:---:|---|---|---:|---|
| READ   |   | `[RA] = READ()`         | PC++          | 100 ![RED](https://placehold.co/15x15/red/red.png) | reads input from the user |
| WRITE  |   | `print([RA])`           | PC++          | 100 ![RED](https://placehold.co/15x15/red/red.png) | prints output to the descriptor |
| LOAD   | i | `[RA] = MEM[i]`         | PC++          |  50 ![ORANGE](https://placehold.co/15x15/orange/orange.png) |  |
| STORE  | i | `MEM[i] = [RA]`         | PC++          |  50 ![ORANGE](https://placehold.co/15x15/orange/orange.png) |  |
| RLOAD  | x | `[RA] = MEM[RX]`        | PC++          |  50 ![ORANGE](https://placehold.co/15x15/orange/orange.png) |  |
| RSTORE | x | `MEM[RX] = [RA]`        | PC++          |  50 ![ORANGE](https://placehold.co/15x15/orange/orange.png) |  |
| ADD    | x | `[RA] = [RA] + [RX]`    | PC++          |   5 ![YELLOW](https://placehold.co/15x15/yellow/yellow.png) |  |
| SUB    | x | `[RA] = [RA] - [RX]`    | PC++          |   5 ![YELLOW](https://placehold.co/15x15/yellow/yellow.png) |  |
| SWP    | x | `swap([RA], [RX])`      | PC++          |   5 ![YELLOW](https://placehold.co/15x15/yellow/yellow.png) |  |
| RST    | x | `[RX] = 0`              | PC++          |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| INC    | x | `[RX]++`                | PC++          |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| DEC    | x | `[RA]--`                | PC++          |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| SHL    | x | `[RX] = [RX] << 1`      | PC++          |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| SHR    | x | `[RX] = [RX] >> 1`      | PC++          |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| JUMP   | i | `JUMP i`                | PC = i        |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| JPOS   | i | `JNZ i`                 | **cond**      |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| JZERO  | i | `JZ  i`                 | **cond**      |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| CALL   | i | `[RA] = PC+1`, `JUMP i` | **PC = i**    |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| RTRN   |   | `PC = [RA]`             | **PC = [RA]** |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| HALT   |   | `exit`                  | *stop*        |   0 ![CYAN](https://placehold.co/15x15/cyan/cyan.png) | stops the program |