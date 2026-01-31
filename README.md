# fltt-compiler
Compiler project for the Formal Languages and Translation Techniques course in the 5th semester at Wrocław University of Science and Technology


# Project structure
`src/` - compiler source code  
`mw2025/` - virtual machine (old) by [prof Maciej Gębala](https://cs.pwr.edu.pl/gebala/)  
`mw2025-p/` - virtual machine by [prof Maciej Gębala](https://cs.pwr.edu.pl/gebala/)  
`tests/` - test sets: see [testing](#testing)  


# Building

required:
 - `c++23` or higher
 - `cmake version ≥ 3.31`

required libraries must be installed: 
 - [Bison parser](https://www.gnu.org/software/bison/)
 - [Flex lexer](https://github.com/westes/flex)
 - [CLN](https://www.ginac.de/CLN/)

```sh
sudo apt-get update
sudo apt-get install flex
sudo apt-get install bison
sudo apt-get install libcln-dev
```

dependencies:
 - p-ranav's [argparse](https://github.com/p-ranav/argparse) (downloaded with FetchContent)
 - fltt-compiler-tools (submodule)

```sh
git clone https://github.com/CoconutOnPalm/fltt-compiler.git
cd fltt-compiler

cmake -S ./ -B ./build/
cmake --build ./build/ --parallel

# compile virtual machine
make -C ./mw2025-p/
```

```sh
# optional: init fltt-compiler-tools
git submodule init
git submodule update
cmake -S ./fltt-compiler-tools/ -B ./fltt-compiler-tools/build/
cmake --build ./fltt-compiler-tools/build/ --parallel
```
see the [fltt-compiler-tools](https://github.com/CoconutOnPalm/fltt-compiler-tools) repository for more info

> [!IMPORTANT]
> to make the benchmarking tool work, you must modify the `fltt-compiler-tools/benchmarker/config.json` to match to current directory
>

```json
// fltt-compiler-tools/benchmarker/config.json:
{
    "benchmarks-dir": "fltt-compiler-tools/benchmarker/programs",
    "benchmark-table": "fltt-compiler-tools/benchmarker/benchmark-table.json",
    "compiler-exe": "kompilator",
    "compiled-dir": "fltt-compiler-tools/benchmarker/.compiled"
}
```

# Testing

requirements: Python 3

```sh
python tests/run-tests.py
```

> test set inside `tests/gijoop-tests/` was kindly provided by [gijoop](https://github.com/gijoop/jftt-compiler)

# Usage

```sh
./kompilator src-file.imp asm-file.mr
```
run with
```sh
./mw2025-p/maszyna-wirtualna-cln asm-file.mr
```

# Language Structure

The structure is described by the fltt-compiler's language, some parameters are encapsulated in the `< >` brackets

**grammar:**
```
program_all  -> procedures main

procedures   -> procedures PROCEDURE proc_head IS declarations IN commands END
             | procedures PROCEDURE proc_head IS IN commands END
             | 

main         -> PROGRAM IS declarations IN commands END
             | PROGRAM IS IN commands END

commands     -> commands command
             | command

command      -> identifier := expression;
             | IF condition THEN commands ELSE commands ENDIF
             | IF condition THEN commands ENDIF
             | WHILE condition DO commands ENDWHILE
             | REPEAT commands UNTIL condition;
             | FOR pidentifier FROM value TO value DO commands ENDFOR
             | FOR pidentifier FROM value DOWNTO value DO commands ENDFOR
             | proc_call;
             | READ identifier;
             | WRITE value;

proc_head    -> pidentifier ( args_decl )

proc_call    -> pidentifier ( args )

declarations -> declarations, pidentifier
             | declarations, pidentifier[num:num]
             | pidentifier
             | pidentifier[num:num]

args_decl    -> args_decl, type pidentifier
             | type pidentifier
             
type         -> T | I | O | ε

args         -> args, pidentifier
             | pidentifier

expression   -> value
             | value + value
             | value - value
             | value * value
             | value / value
             | value % value

condition    -> value = value
             | value != value
             | value > value
             | value < value
             | value >= value
             | value <= value

value        -> num
             | identifier

identifier   -> pidentifier
             | pidentifier[pidentifier]
             | pidentifier[num]
```

```
where:
    pidentifier:    [a-z_]+
    num:            64-bit unsigned integer
```


# Examples

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

technically this should be called a procedure, but following the standard c-like naming convention, I'll using `function` instead of `procedure` do describe procedures in this documentation

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
   - $x, y \in [0..inf)$
   - $x<0 → x=0$ 
   - $\frac{x}{0}=0,\ rem=0$  
```
example:
2 - 3 = 0
2 / 3 = 0
2 / 0 = 0
2 % 0 = 0
```

## 2. `arr[a:b]` declares an array of indices $[a, a+1, ..., b]$ (inclusive)
```
example: 
arr[3, 7] = array of size 5, ind: [3, 4, 5, 6, 7]
```

## 3. No recursion

## 4. Declarations
All variables must be declared inside a function, or be passed as `IN/OUT` ref arguments
 - **`I`** (IN) param marks an argument as `readonly const`
 - **`O`** (OUT) param marks an argument as an 'OUT' argument - cannot be read before assignment, cannot be passed as `I`
 - **`T`** marks an argument as an array

## 5. for loops
FOR loop has a non-modifiable local iterator which can only be incremented by ±1

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
let `MEM[i]` be the memory cell at address *`i`*  

|instruction|arg|action|PC|cost|description|
|:---|:---:|---|---|---:|---|
| READ   |   | `[RA] = READ()`         | PC++          | 100 ![RED](https://placehold.co/15x15/red/red.png) | reads input from stdin |
| WRITE  |   | `print([RA])`           | PC++          | 100 ![RED](https://placehold.co/15x15/red/red.png) | prints output stdout |
| LOAD   | i | `[RA] = MEM[i]`         | PC++          |  50 ![ORANGE](https://placehold.co/15x15/orange/orange.png) |  |
| STORE  | i | `MEM[i] = [RA]`         | PC++          |  50 ![ORANGE](https://placehold.co/15x15/orange/orange.png) |  |
| RLOAD  | x | `[RA] = MEM[RX]`        | PC++          |  50 ![ORANGE](https://placehold.co/15x15/orange/orange.png) |  |
| RSTORE | x | `MEM[RX] = [RA]`        | PC++          |  50 ![ORANGE](https://placehold.co/15x15/orange/orange.png) |  |
| ADD    | x | `[RA] = [RA] + [RX]`    | PC++          |   5 ![YELLOW](https://placehold.co/15x15/yellow/yellow.png) |  |
| SUB    | x | \*`[RA] = [RA] - [RX]`  | PC++          |   5 ![YELLOW](https://placehold.co/15x15/yellow/yellow.png) |  |
| SWP    | x | `swap([RA], [RX])`      | PC++          |   5 ![YELLOW](https://placehold.co/15x15/yellow/yellow.png) |  |
| RST    | x | `[RX] = 0`              | PC++          |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| INC    | x | `[RX]++`                | PC++          |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| DEC    | x | \*`[RX]--`              | PC++          |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| SHL    | x | `[RX] = [RX] << 1`      | PC++          |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| SHR    | x | `[RX] = [RX] >> 1`      | PC++          |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| JUMP   | i | `JUMP i`                | PC = i        |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| JPOS   | i | `JNZ i`                 | **cond**      |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| JZERO  | i | `JZ  i`                 | **cond**      |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| CALL   | i | `[RA] = PC+1`, `JUMP i` | **PC = i**    |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| RTRN   |   | `PC = [RA]`             | **PC = [RA]** |   1 ![GREEN](https://placehold.co/15x15/green/green.png) |  |
| HALT   |   | `exit`                  | *stop*        |   0 ![CYAN](https://placehold.co/15x15/cyan/cyan.png) | stops the program |

\* *where* `[RA]`, `[RB]`, `...`, `[RH]` hold unsigned integers (of any size)

*the compiler itself does not contain any AI-generated code*