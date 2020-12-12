# ![(**N**)](figures/big-n.svg)

![(**N**)](figures/n.svg) is a primitive recursive programming language which operates on finite sequences of natural numbers.

Inspired by the recombinant nature of genetic code, along with programming languages like Douglas Hofstadter's *BlooP* and Urban Müller's *Brainfuck*, ![(**N**)](figures/n.svg) was designed with the goal of creating a language in which any permutation of code represents a valid program, and every program is *provably terminating*.

## Table of Contents

1. [Programs](#programs)
2. [Syntax](#syntax)
3. [Examples](#examples)
4. [Tools](#tools)
5. [Algorithms](#algorithms)
6. [Constants](#constants)
7. [License](#license)

## Programs

An ![(**N**)](figures/n.svg) program transforms a sequence of natural numbers. The only input to a program is its initial sequence, and the only output is its final sequence. If no input sequence is given, the initial sequence will be the zero singleton, (0).

The only control flow structure in ![(**N**)](figures/n.svg) is a bounded loop, meaning that the number of times a loop is repeated is determined before the loop is entered, and cannot be changed from inside the loop itself. This results in all ![(**N**)](figures/n.svg) programs being *provably terminating*.

## Syntax

There are no syntax errors in ![(**N**)](figures/n.svg); the syntax of ![(**N**)](figures/n.svg) was designed such that any permutation of operators represents a valid program. Characters other than operators and comment delimeters are ignored.

### Operators

Operators in ![(**N**)](figures/n.svg) operate on either the first element, last element, or the sequence as a whole; there are no binary operators. A table of all nine operators and their descriptions is given below:

| Operator | Name        | Description                                                                                       |
|:--------:|:------------|:--------------------------------------------------------------------------------------------------|
|   `+`    | Increment   | Increment the value of the first element.                                                         |
|   `-`    | Decrement   | Decrement the value of the first element, if non-zero.                                            |
|   `#`    | Count       | Set the value of the first element to the current length of the sequence.                         |
|   `>`    | Shift right | Right circular shift all elements by one position.                                                |
|   `<`    | Shift left  | Left circular shift all elements by one position.                                                 |
|   `:`    | Append      | Append the value of the first element to the end of the sequence.                                 |
|  `\|`    | Truncate    | Remove the last element from the sequence, if the sequence has more than one element.             |
|   `[`    | Loop start  | Set loop counter to the value of the first element, then jump past the matching loop end if zero. |
|   `]`    | Loop end    | Decrement loop counter then jump back to the start of the loop if non-zero.                       |

### Comments

The comment delimeter is represented by a semicolon, `;`. All characters following a comment delimeter will be ignored until the line ends.

## Examples

This sections contains a few example programs written in ![(**N**)](figures/n.svg). For more examples, see the [examples](./examples) directory.

### Hello, World! (Unoptimized)

```.bf
; hello.n
#[|-]             ; Clear initial sequence
:::::::::::       ; Add 11 zero elements
++[++[+++]]<      ;  72 | H
++[[-[+]]+++]<    ; 101 | e
+++[-[+++]]       ; 108 | l
:<                ; 108 | l
++++[+[+]+]++<    ; 111 | o
+++[+[+]+]-<      ;  44 | ,
++[[+++]]<        ;  32 |  
+++[-[+++]-]<     ;  87 | W
++++[+[+]+]++<    ; 111 | o
++[+[+++++]]<     ; 114 | r
+++[-[+++]]<      ; 108 | l
+++++[-[+]]++<    ; 100 | d
++[[+++]]+<       ;  33 | !
```

### Factorial

```.bf
; factorial.n
:<#[<|]           ; Isolate first element, x
:>[-]:+<          ; Add two elements, z = 1, y = 0
[                 ; for (j = x; j; --j) {
    [>[>+<]<]     ;     y += x * z
    >[-]>[<+>-]>  ;     z = y, y = 0
    -             ;     --x
]                 ; }
>||               ; return z
```

### Fibonacci Sequence

```.bf
; fibonacci.n
:<#[<|]           ; Isolate first element, x
::                ; Add two elements, y and z
<[-]+             ; y = 1
<[[-]+]           ; z = (x > 0) ? 1 : 0;
<--               ; x -= 2
[                 ; for (j = x; j; --j) {
    [-]>[<+>]<    ;     x = z
    <[<+>]        ;     z += y
    [-]>[<+>]     ;     y = x
]                 ; }
>||               ; return z
```

## Tools

The tools provided in this repository include an ![(**N**)](figures/n.svg) interpreter, an ![(**N**)](figures/n.svg) to C translator, and a binary to ![(**N**)](figures/n.svg) converter. The provided tools are written in C, and can be built with CMake using the following commands:

```.sh
cd build
cmake .. -G <generator-name> -DCMAKE_BUILD_Type=[Release, Debug] 
cmake --build .
```

### nterpreter

*nterpreter* is an ![(**N**)](figures/n.svg) interpreter. The usage of *nterpreter* is as follows:

```.sh
n <source file> [options] [first element] ... [last element]
```

#### Options:

* `--output, -o <file>`: Write output sequence to a file.
* `--input-numbers,  -in`: Read input sequence as a series of numbers.
* `--input-bytes,    -ib`: Read input sequence as a series of bytes.
* `--output-numbers, -on`: Write output sequence as a series of numbers.
* `--output-bytes,   -ob`: Write output sequence as a series of bytes.

### n2c

*n2c* is a tool which can translate ![(**N**)](figures/n.svg) source code into compilable C source code. The usage of *n2c* is as follows:

```.sh
n2c <input file> [output file]
```

### bin2n

*bin2n* is a tool which converts any binary file into an ![(**N**)](figures/n.svg) program which, when executed, will reproduce the exact sequence of bytes which made up the binary file. The usage of *bin2n* is as follows:

```.sh
bin2n <input file> [output file]
```

## Algorithms

The following table contains common algorithms in ![(**N**)](figures/n.svg):

| Name                        | Input       | Output              | Operations                                          |
|:----------------------------|:------------|:--------------------|:----------------------------------------------------|
| x = 0                       | (x, ...)    | (0, ...)            | `[-]`                                               |
| x = 1                       | (x, ...)    | (1, ...)            | `[-]+`                                              |
| x = y                       | (x, y, ...) | (y, y, ...)         | `[-]<[>+<]>`                                        |
| x = x + y                   | (x, y, ...) | (x + y, y, ...)     | `<[>+<]>`                                           |
| x = x - y                   | (x, y, ...) | (x - y, y, ...)     | `<[>-<]>`                                           |
| x = x * y                   | (x, y, ...) | (x * y, y, ...)     | `:[-]>[<<[>+<]>>]<\|`                               |
| x = x / y                   | (x, y, ...) | (x / y, y, ...)     | `:+>[-]<[<[>-<]>:>[[-]+][<\|>+<:>]<\|]<\|>`         |
| x = x^2                     | (x, ...)    | (x^2, ...)          | `:[-]>[[<+>]]<\|`                                   |
| x = not x                   | (x, ...)    | (not x, ...)        | `:[-]+>[<->]<\|`                                    |
| x = not not x               | (x, ...)    | (not not x, ...)    | `[[-]+]`                                            |
| x = x or y                  | (x, y, ...) | (x or y, y, ...)    | `<[>+<]>[[-]+]`                                     |
| x = x and y                 | (x, y, ...) | (x and y, y, ...)   | `:[-]>[<<[>+<]>>]<\|[[-]+]`                         |
| swap(x, y)                  | (x, y, ...) | (y, x, ...)         | `:>[-]<<[>>+<<]<\|>>`                               |
| delete x                    | (x, ...)    | (...)               | `<\|`                                               |
| delete y                    | (x, y, ...) | (x, ...)            | `<<\|>`                                             |
| clear                       | (...)       | (0)                 | `#[\|-]`                                            |
| isolate x                   | (x, ...)    | (x)                 | `:<#[<\|]`                                          |
| if (x) {...}                | (x, ...)    | (x, ...)            | `:>[[-]+][<\|` {...} `:>]<\|`                       |
| if (not x) {...}            | (x, ...)    | (x, ...)            | `:>:[-]+>[<->]<\|[<\|` {...} `:>]<\|`               |
| if (x > y) {...}            | (x, y, ...) | (x, y, ...)         | `:<[>>-<<]>>[[-]+][<\|` {...} `:>]<\|`              |
| if (x < y) {...}            | (x, y, ...) | (x, y, ...)         | `:>[-]<<[>>+<<]>[>-<]>[[-]+][<\|` {...} `:>]<\|`    |
| if (x >= y) {...}           | (x, y, ...) | (x, y, ...)         | `+:-<[>>-<<]>>[[-]+][<\|` {...} `:>]<\|`            |
| if (x <= y) {...}           | (x, y, ...) | (x, y, ...)         | `:>[-]<<[>>+<<]>>+<[>-<]>[[-]+][<\|` {...} `:>]<\|` |
| for each element {...}      | (...)       | (...)               | `:>#-[<\|` {...} `:]<\|`                            |

## Constants

The following tables list the fewest possible single-element operations for various numbers:

### 8-bit (0, 1, 2, 3, ..., 255)

| Dec | Hex  | ASCII | Operations         | Length |
|:---:|:----:|:-----:|:-------------------|-------:|
|   0 | 0x00 |  NUL  |                    |      0 |
|   1 | 0x01 |  SOH  | `+`                |      1 |
|   2 | 0x02 |  STX  | `++`               |      2 |
|   3 | 0x03 |  ETX  | `+++`              |      3 |
|   4 | 0x04 |  EOT  | `++++`             |      4 |
|   5 | 0x05 |  ENQ  | `+++++`            |      5 |
|   6 | 0x06 |  ACK  | `++++++`           |      6 |
|   7 | 0x07 |  BEL  | `+++++++`          |      7 |
|   8 | 0x08 |  BS   | `++++[+]`          |      7 |
|   9 | 0x09 |  TAB  | `+++[++]`          |      7 |
|  10 | 0x0A |  LF   | `+++++[+]`         |      8 |
|  11 | 0x0B |  VT   | `++[[+]+]`         |      8 |
|  12 | 0x0C |  FF   | `++++[++]`         |      8 |
|  13 | 0x0D |  CR   | `++++[++]+`        |      9 |
|  14 | 0x0E |  SO   | `++[+[+]]`         |      8 |
|  15 | 0x0F |  SI   | `+++++[++]`        |      9 |
|  16 | 0x10 |  DLE  | `++++[+++]`        |      9 |
|  17 | 0x11 |  DC1  | `+++[[+]-]`        |      9 |
|  18 | 0x12 |  DC2  | `++[[++]]`         |      8 |
|  19 | 0x13 |  DC3  | `++[[++]]+`        |      9 |
|  20 | 0x14 |  DC4  | `++[++[+]]`        |      9 |
|  21 | 0x15 |  NAK  | `++[++[+]]+`       |     10 |
|  22 | 0x16 |  SYN  | `++[[++]+]`        |      9 |
|  23 | 0x17 |  ETB  | `+++[[+]]-`        |      9 |
|  24 | 0x18 |  CAN  | `+++[[+]]`         |      8 |
|  25 | 0x19 |  EM   | `+++[[+]]+`        |      9 |
|  26 | 0x1A |  SUB  | `+++[[+]]++`       |     10 |
|  27 | 0x1B |  ESC  | `++[[+++]-]`       |     10 |
|  28 | 0x1C |  FS   | `++[+[++]]--`      |     11 |
|  29 | 0x1D |  GS   | `++[+[++]]-`       |     10 |
|  30 | 0x1E |  RS   | `++[+[++]]`        |      9 |
|  31 | 0x1F |  US   | `+++[[+]+]`        |      9 |
|  32 | 0x20 | Space | `++[[+++]]`        |      9 |
|  33 | 0x21 |   !   | `++[[+++]]+`       |     10 |
|  34 | 0x22 |   "   | `++++[-[+]]`       |     10 |
|  35 | 0x23 |   #   | `++++[-[+]]+`      |     11 |
|  36 | 0x24 |   $   | `+++[+[+]]--`      |     11 |
|  37 | 0x25 |   %   | `+++[+[+]]-`       |     10 |
|  38 | 0x26 |   &   | `+++[+[+]]`        |      9 |
|  39 | 0x27 |   '   | `+++[+[+]]+`       |     10 |
|  40 | 0x28 |   (   | `+++[+[+]]++`      |     11 |
|  41 | 0x29 |   )   | `+++[-[++]]-`      |     11 |
|  42 | 0x2A |  \*   | `+++[-[++]]`       |     10 |
|  43 | 0x2B |   +   | `+++[-[++]]+`      |     11 |
|  44 | 0x2C |   ,   | `+++[+[+]+]-`      |     11 |
|  45 | 0x2D |   -   | `+++[+[+]+]`       |     10 |
|  46 | 0x2E |   .   | `+++[+[+]+]+`      |     11 |
|  47 | 0x2F |   /   | `++[+[+++]-]`      |     11 |
|  48 | 0x30 |   0   | `++++[[+]-]-`      |     11 |
|  49 | 0x31 |   1   | `++++[[+]-]`       |     10 |
|  50 | 0x32 |   2   | `++[[++++]]`       |     10 |
|  51 | 0x33 |   3   | `+++[++[+]]-`      |     11 |
|  52 | 0x34 |   4   | `+++[++[+]]`       |     10 |
|  53 | 0x35 |   5   | `+++[++[+]]+`      |     11 |
|  54 | 0x36 |   6   | `++[+++[++]]`      |     11 |
|  55 | 0x37 |   7   | `+++[-[++]+]`      |     11 |
|  56 | 0x38 |   8   | `++[[++++]+]`      |     11 |
|  57 | 0x39 |   9   | `++[+[+++]+]`      |     11 |
|  58 | 0x3A |   :   | `+++[++[+]+]-`     |     12 |
|  59 | 0x3B |   ;   | `+++[++[+]+]`      |     11 |
|  60 | 0x3C |   <   | `+++[++[+]+]+`     |     12 |
|  61 | 0x3D |   =   | `++++[[+]]---`     |     12 |
|  62 | 0x3E |   >   | `++++[[+]]--`      |     11 |
|  63 | 0x3F |   ?   | `++++[[+]]-`       |     10 |
|  64 | 0x40 |   @   | `++++[[+]]`        |      9 |
|  65 | 0x41 |   A   | `++++[[+]]+`       |     10 |
|  66 | 0x42 |   B   | `++++[[+]]++`      |     11 |
|  67 | 0x43 |   C   | `+++[[++]-]-`      |     11 |
|  68 | 0x44 |   D   | `+++[[++]-]`       |     10 |
|  69 | 0x45 |   E   | `+++[[++]-]+`      |     11 |
|  70 | 0x46 |   F   | `+++[[++]-]++`     |     12 |
|  71 | 0x47 |   G   | `++[++[+++]]-`     |     12 |
|  72 | 0x48 |   H   | `++[++[+++]]`      |     11 |
|  73 | 0x49 |   I   | `+++[+++[+]+]`     |     12 |
|  74 | 0x4A |   J   | `++[+[++++]-]`     |     12 |
|  75 | 0x4B |   K   | `+++[+[+]][+]-`    |     13 |
|  76 | 0x4C |   L   | `+++[+[+]][+]`     |     12 |
|  77 | 0x4D |   M   | `++++[[+]+]--`     |     12 |
|  78 | 0x4E |   N   | `++++[[+]+]-`      |     11 |
|  79 | 0x4F |   O   | `++++[[+]+]`       |     10 |
|  80 | 0x50 |   P   | `+++[[++]]-`       |     10 |
|  81 | 0x51 |   Q   | `+++[[++]]`        |      9 |
|  82 | 0x52 |   R   | `+++[[++]]+`       |     10 |
|  83 | 0x53 |   S   | `+++[[++]]++`      |     11 |
|  84 | 0x54 |   T   | `++++[--[++]]`     |     12 |
|  85 | 0x55 |   U   | `++++[--[++]]+`    |     13 |
|  86 | 0x56 |   V   | `++[+[++++]+]`     |     12 |
|  87 | 0x57 |   W   | `+++[-[+++]-]`     |     12 |
|  88 | 0x58 |   X   | `+++[-[+++]-]+`    |     13 |
|  89 | 0x59 |   Y   | `+++[+[+]+][+]-`   |     14 |
|  90 | 0x5A |   Z   | `+++[+[+]+][+]`    |     13 |
|  91 | 0x5B |  \[   | `++++[+[+]]---`    |     13 |
|  92 | 0x5C |  \\   | `++++[+[+]]--`     |     12 |
|  93 | 0x5D |  \]   | `++++[+[+]]-`      |     11 |
|  94 | 0x5E |   ^   | `++++[+[+]]`       |     10 |
|  95 | 0x5F |   _   | `++++[+[+]]+`      |     11 |
|  96 | 0x60 |  \`   | `++++[+[+]]++`     |     12 |
|  97 | 0x61 |   a   | `+++++[-[+]]-`     |     12 |
|  98 | 0x62 |   b   | `+++++[-[+]]`      |     11 |
|  99 | 0x63 |   c   | `+++++[-[+]]+`     |     12 |
| 100 | 0x64 |   d   | `+++++[-[+]]++`    |     13 |
| 101 | 0x65 |   e   | `++[[-[+]]+++]`    |     13 |
| 102 | 0x66 |   f   | `++++[-[+]][++]`   |     14 |
| 103 | 0x67 |   g   | `+++[++[+]][+]-`   |     14 |
| 104 | 0x68 |   h   | `+++[++[+]][+]`    |     13 |
| 105 | 0x69 |   i   | `+++[+[++]-]--`    |     13 |
| 106 | 0x6A |   j   | `+++[+[++]-]-`     |     12 |
| 107 | 0x6B |   k   | `+++[+[++]-]`      |     11 |
| 108 | 0x6C |   l   | `+++[-[+++]]`      |     11 |
| 109 | 0x6D |   m   | `++++[+[+]+]`      |     11 |
| 110 | 0x6E |   n   | `++++[+[+]+]+`     |     12 |
| 111 | 0x6F |   o   | `++++[+[+]+]++`    |     13 |
| 112 | 0x70 |   p   | `++[++++[+++]]`    |     13 |
| 113 | 0x71 |   q   | `++[+[+++++]]-`    |     13 |
| 114 | 0x72 |   r   | `++[+[+++++]]`     |     12 |
| 115 | 0x73 |   s   | `++[+[+++++]]+`    |     13 |
| 116 | 0x74 |   t   | `++[++[++++]+]`    |     13 |
| 117 | 0x75 |   u   | `+++[+[++]]---`    |     13 |
| 118 | 0x76 |   v   | `+++[+[++]]--`     |     12 |
| 119 | 0x77 |   w   | `+++[+[++]]-`      |     11 |
| 120 | 0x78 |   x   | `+++[+[++]]`       |     10 |
| 121 | 0x79 |   y   | `+++[+[++]]+`      |     11 |
| 122 | 0x7A |   z   | `+++[+[++]]++`     |     12 |
| 123 | 0x7B |   {   | `++++[++[+]]-`     |     12 |
| 124 | 0x7C |  \|   | `++++[++[+]]`      |     11 |
| 125 | 0x7D |   }   | `++++[++[+]]+`     |     12 |
| 126 | 0x7E |   ~   | `++++[++[+]]++`    |     13 |
| 127 | 0x7F |  DEL  | `++[[[+]-]]--`     |     12 |
| 128 | 0x80 |       | `++[[[+]-]]-`      |     11 |
| 129 | 0x81 |       | `++[[[+]-]]`       |     10 |
| 130 | 0x82 |       | `++[[[+]-]]+`      |     11 |
| 131 | 0x83 |       | `++[[[+]-]]++`     |     12 |
| 132 | 0x84 |       | `+++[+[++]+]-`     |     12 |
| 133 | 0x85 |       | `+++[+[++]+]`      |     11 |
| 134 | 0x86 |       | `+++[+[++]+]+`     |     12 |
| 135 | 0x87 |       | `+++[+[++]+]++`    |     13 |
| 136 | 0x88 |       | `+++[[++]-][+]`    |     13 |
| 137 | 0x89 |       | `++[[+++][+]+]`    |     13 |
| 138 | 0x8A |       | `++++[++[+]+]-`    |     13 |
| 139 | 0x8B |       | `++++[++[+]+]`     |     12 |
| 140 | 0x8C |       | `++++[++[+]+]+`    |     13 |
| 141 | 0x8D |       | `++++[++[+]+]++`   |     14 |
| 142 | 0x8E |       | `++[++[++]-[+]]`   |     14 |
| 143 | 0x8F |       | `++[++[++]-[+]]+`  |     15 |
| 144 | 0x90 |       | `+++[++[++]-]--`   |     14 |
| 145 | 0x91 |       | `+++[++[++]-]-`    |     13 |
| 146 | 0x92 |       | `+++[++[++]-]`     |     12 |
| 147 | 0x93 |       | `+++[++[++]-]+`    |     13 |
| 148 | 0x94 |       | `+++[++[++]-]++`   |     14 |
| 149 | 0x95 |       | `+++[[+++]--]-`    |     13 |
| 150 | 0x96 |       | `+++[[+++]--]`     |     12 |
| 151 | 0x97 |       | `+++[[+++]--]+`    |     13 |
| 152 | 0x98 |       | `++[[++][++]-]`    |     13 |
| 153 | 0x99 |       | `++++[+++[+]]-`    |     13 |
| 154 | 0x9A |       | `++++[+++[+]]`     |     12 |
| 155 | 0x9B |       | `++++[+++[+]]+`    |     13 |
| 156 | 0x9C |       | `++[++[+++++]]`    |     13 |
| 157 | 0x9D |       | `++[[[+]]---]`     |     12 |
| 158 | 0x9E |       | `+++++[[+]]--`     |     12 |
| 159 | 0x9F |       | `+++++[[+]]-`      |     11 |
| 160 | 0xA0 |       | `+++++[[+]]`       |     10 |
| 161 | 0xA1 |       | `+++++[[+]]+`      |     11 |
| 162 | 0xA2 |       | `+++++[[+]]++`     |     12 |
| 163 | 0xA3 |       | `+++++[[+]]+++`    |     13 |
| 164 | 0xA4 |       | `++++[-[++]-]`     |     12 |
| 165 | 0xA5 |       | `++++[-[++]-]+`    |     13 |
| 166 | 0xA6 |       | `++++[-[++]-]++`   |     14 |
| 167 | 0xA7 |       | `++++[-[++]-]+++`  |     15 |
| 168 | 0xA8 |       | `++++[+++[+]+]-`   |     14 |
| 169 | 0xA9 |       | `++++[+++[+]+]`    |     13 |
| 170 | 0xAA |       | `+++[[+++]-]-`     |     12 |
| 171 | 0xAB |       | `+++[[+++]-]`      |     11 |
| 172 | 0xAC |       | `+++[++[++]+]`     |     12 |
| 173 | 0xAD |       | `+++[++[++]+]+`    |     13 |
| 174 | 0xAE |       | `+++[++[++]+]++`   |     14 |
| 175 | 0xAF |       | `+++[++[++]+]+++`  |     15 |
| 176 | 0xB0 |       | `++[++[+]+[++]]-`  |     15 |
| 177 | 0xB1 |       | `++[++[+]+[++]]`   |     14 |
| 178 | 0xB2 |       | `++[[++++]-[+]]`   |     14 |
| 179 | 0xB3 |       | `++[[++++]-[+]]+`  |     15 |
| 180 | 0xB4 |       | `+++[+[+]+][+++]`  |     15 |
| 181 | 0xB5 |       | `++[+[+++]-[+]]-`  |     15 |
| 182 | 0xB6 |       | `++[+[+++]-[+]]`   |     14 |
| 183 | 0xB7 |       | `++++[++++[+]]-`   |     14 |
| 184 | 0xB8 |       | `++++[++++[+]]`    |     13 |
| 185 | 0xB9 |       | `+++[+++[++]-]`    |     13 |
| 186 | 0xBA |       | `++++[+[+]]-[+]`   |     14 |
| 187 | 0xBB |       | `++++[+[+]][+]-`   |     14 |
| 188 | 0xBC |       | `++++[+[+]][+]`    |     13 |
| 189 | 0xBD |       | `+++++[[+]+]--`    |     13 |
| 190 | 0xBE |       | `+++++[[+]+]-`     |     12 |
| 191 | 0xBF |       | `+++++[[+]+]`      |     11 |
| 192 | 0xC0 |       | `+++[[+++]]`       |     10 |
| 193 | 0xC1 |       | `+++[[+++]]+`      |     11 |
| 194 | 0xC2 |       | `+++[[+++]]++`     |     12 |
| 195 | 0xC3 |       | `++++++[-[+]-]`    |     13 |
| 196 | 0xC4 |       | `++++++[-[+]-]+`   |     14 |
| 197 | 0xC5 |       | `+++[+++[++]]-`    |     13 |
| 198 | 0xC6 |       | `+++[+++[++]]`     |     12 |
| 199 | 0xC7 |       | `+++[+++[++]]+`    |     13 |
| 200 | 0xC8 |       | `++[+[+++][+]]`    |     13 |
| 201 | 0xC9 |       | `++++[-[++]]---`   |     14 |
| 202 | 0xCA |       | `++++[-[++]]--`    |     13 |
| 203 | 0xCB |       | `++++[-[++]]-`     |     12 |
| 204 | 0xCC |       | `++++[-[++]]`      |     11 |
| 205 | 0xCD |       | `++++[-[++]]+`     |     12 |
| 206 | 0xCE |       | `++++[-[++]]++`    |     13 |
| 207 | 0xCF |       | `++++[-[++]]+++`   |     14 |
| 208 | 0xD0 |       | `++++[-[++]]++++`  |     15 |
| 209 | 0xD1 |       | `++[+[+++][+]+]`   |     14 |
| 210 | 0xD2 |       | `+++[+++[++]+]-`   |     14 |
| 211 | 0xD3 |       | `+++[+++[++]+]`    |     13 |
| 212 | 0xD4 |       | `+++[[+++]+]-`     |     12 |
| 213 | 0xD5 |       | `+++[[+++]+]`      |     11 |
| 214 | 0xD6 |       | `+++[[+++]+]+`     |     12 |
| 215 | 0xD7 |       | `+++[[+++]+]++`    |     13 |
| 216 | 0xD8 |       | `+++[-[+++]][+]`   |     14 |
| 217 | 0xD9 |       | `+++[-[+++]][+]+`  |     15 |
| 218 | 0xDA |       | `++++[+[+]+][+]`   |     14 |
| 219 | 0xDB |       | `+++[-[++++]]-`    |     13 |
| 220 | 0xDC |       | `+++[-[++++]]`     |     12 |
| 221 | 0xDD |       | `+++++[+[+]]-`     |     12 |
| 222 | 0xDE |       | `+++++[+[+]]`      |     11 |
| 223 | 0xDF |       | `+++++[+[+]]+`     |     12 |
| 224 | 0xE0 |       | `+++++[+[+]]++`    |     13 |
| 225 | 0xE1 |       | `+++++[+[+]]+++`   |     14 |
| 226 | 0xE2 |       | `+++++[+[+]]++++`  |     15 |
| 227 | 0xE3 |       | `++[+[+++]+[+]+]`  |     15 |
| 228 | 0xE4 |       | `++[+[+++++]][+]`  |     15 |
| 229 | 0xE5 |       | `++++[+++++[+]+]`  |     15 |
| 230 | 0xE6 |       | `++++[+++++[+]+]+` |     16 |
| 231 | 0xE7 |       | `+++[[+++]++]---`  |     15 |
| 232 | 0xE8 |       | `+++[[+++]++]--`   |     14 |
| 233 | 0xE9 |       | `+++[[+++]++]-`    |     13 |
| 234 | 0xEA |       | `+++[[+++]++]`     |     12 |
| 235 | 0xEB |       | `+++[[+++]++]+`    |     13 |
| 236 | 0xEC |       | `+++[++++[++]]-`   |     14 |
| 237 | 0xED |       | `+++[++++[++]]`    |     13 |
| 238 | 0xEE |       | `+++[++++[++]]+`   |     14 |
| 239 | 0xEF |       | `+++[+[++]][+]-`   |     14 |
| 240 | 0xF0 |       | `+++[+[++]][+]`    |     13 |
| 241 | 0xF1 |       | `+++[+[++]][+]+`   |     14 |
| 242 | 0xF2 |       | `++++[-[++]+]--`   |     14 |
| 243 | 0xF3 |       | `++++[-[++]+]-`    |     13 |
| 244 | 0xF4 |       | `++++[-[++]+]`     |     12 |
| 245 | 0xF5 |       | `++++[-[++]+]+`    |     13 |
| 246 | 0xF6 |       | `++++[-[++]+]++`   |     14 |
| 247 | 0xF7 |       | `++++[-[++]+]+++`  |     15 |
| 248 | 0xF8 |       | `++++[++[+]][+]`   |     14 |
| 249 | 0xF9 |       | `++[[+++]-[++]]`   |     14 |
| 250 | 0xFA |       | `+++[++++[++]+]`   |     14 |
| 251 | 0xFB |       | `+++[-[++++]+]`    |     13 |
| 252 | 0xFC |       | `+++++[+[+]+]-`    |     13 |
| 253 | 0xFD |       | `+++++[+[+]+]`     |     12 |
| 254 | 0xFE |       | `+++++[+[+]+]+`    |     13 |
| 255 | 0xFF |       | `+++[+[+++]-]`     |     12 |

## License

Copyright © 2020 Christopher J. Howard. This text is licensed under the [Creative Commons Attribution-ShareAlike 4.0 International License](https://creativecommons.org/licenses/by-sa/4.0/). nterpreter, n2c, and bin2n source codes are licensed under the [GNU General Public License, version 3](./LICENSE.md).
