# ![(**N**)](figures/big-n.svg)

![(**N**)](figures/n.svg) is a primitive recursive programming language which operates on finite sequences of natural numbers.

## Table of Contents

1. [Programs](#programs)
2. [Syntax](#syntax)
3. [Examples](#examples)
4. [Algorithms](#algorithms)
5. [Constants](#constants)
6. [Tools](#tools)
7. [License](#license)

## Programs

A program transforms a sequence of natural numbers. The only input to a program is its initial sequence. If no input is given, the initial sequence will be the zero singleton, (0).

The only control flow structure in ![(**N**)](figures/n.svg) is a bounded loop, meaning that all programs are *provably terminating*.

## Syntax

There are no syntax errors in ![(**N**)](figures/n.svg); the syntax of ![(**N**)](figures/n.svg) was designed such that any permutation of operators represents a valid program. Characters other than operators and comment delimeters are ignored.

### Operators

There are ten operators in ![(**N**)](figures/n.svg), listed below:

| Operator | Name             | Description                                                   | C equivalent                   |
|:--------:|:-----------------|:--------------------------------------------------------------|:-------------------------------|
|   `>`    | Next element     | Go to the next element or append a zero to the sequence.      | `++ptr;`                       |
|   `<`    | Previous element | Go to the previous element or prepend a zero to the sequence. | `--ptr;`                       |
|   `i`    | Index            | Set the element value to its index.                           | `*ptr = ptr - head;`           |
|   `#`    | Cardinality      | Set the element value to the sequence length.                 | `*ptr = tail - head + 1;`      |
|   `+`    | Successor        | Increment the element value.                                  | `++*ptr;`                      |
|   `-`    | Predecessor      | Decrement the element value if non-zero.                      | `*ptr -= !!*ptr;`              |
|   `[`    | Start loop       | Repeat the code in the loop *n* times (bounded).              | `for (int i = *ptr; i; --i) {` |
|   `]`    | End loop         | Marks the end of a bounded loop.                              | `}`                            |
|   `(`    | Left closure     | Remove all elements to the left.                              | `head = ptr;`                  |
|   `)`    | Right closure    | Remove all elements to the right.                             | `tail = ptr;`                  |

### Comments

The comment delimeter is represented by a semicolon, `;`. All characters following a comment delimeter will be ignored until the line ends.

```.bf
; n.n
([-])        ; This is
+++[+[+]]++  ; a
>++++[[+]+]- ; comment
>+++[-[++]]- ; example
```

## Examples

### Hello, World! (Naïve)

Annotated:

```.bf
; hello.n
([-])          ; Clear initial sequence
++[++[+++]]>   ;  72 | H 
++[[-[+]]+++]> ; 101 | e
+++[-[+++]]>   ; 108 | l
+++[-[+++]]>   ; 108 | l
++++[+[+]+]++> ; 111 | o
+++[+[+]+]->   ;  44 | ,
++[[+++]]>     ;  32 |  
+++[-[+++]-]>  ;  87 | W
++++[+[+]+]++> ; 111 | o
++[+[+++++]]>  ; 114 | r
+++[-[+++]]>   ; 108 | l
+++++[-[+]]++> ; 100 | d
++[[+++]]+     ;  33 | !
```

Compact:

```.bf
([-])++[++[+++]]>++[[-[+]]+++]>+++[-[+++]]>+++[-[+++]]>++++[+[+]+]++>+++[+[+]+]->++[[+++]]>+++[-[+++]-]>++++[+[+]+]++>++[+[+++++]]>+++[-[+++]]>+++++[-[+]]++>++[[+++]]+
```

### Factorial

Annotated:

```.bf
; factorial.n
()                ; Isolate first element
>+<               ; y = 1
[                 ; for (j = x; j; --j) {
    [>[>+<]<]     ;     z += x * y
    >[-]>[<+>-]<< ;     y = z, z = 0
    -             ;     --x
]>()              ; }
```

Compact:

```.bf
()>+<[[>[>+<]<]>[-]>[<+>-]<<-]>()
```

## Algorithms

| Name          | Input  | Output                  | Operations                       |
|:--------------|:-------|:------------------------|:---------------------------------|
| x = 0         | (x)    | (0)                     | `[-]`                            |
| x = 1         | (x)    | (1)                     | `[-]+`                           |
| x = !x        | (x)    | (~x, ~x)                | `>[-]+<[->-<]>[<+>]<`            |
| x = !!x       | (x)    | (~~x)                   | `[[-]+]`                         |
| x *= x        | (x)    | (x*x, x)                | `>[-]<[>+<-]>[[<+>]]<`           |
| x = y         | (x, y) | (y, y)                  | `[-]>[<+>]<`                     |
| x = !y        | (x, y) | (~y, y)                 | `[-]+>[<[-]>]<`                  |
| x += y        | (x, y) | (x+y, y)                | `>[<+>]<`                        |
| x -= y        | (x, y) | (x-y, y)                | `>[<->]<`                        |
| x *= y        | (x, y) | (x*y, y, x)             | `>>[-]<<[>>+<<-]>>[<[<+>]>-]<<`  |
| y += x        | (x, y) | (x, y+x)                | `[>+<]<`                         |
| y -= x        | (x, y) | (x, y-x)                | `[>-<]<`                         |
| y = x * x     | (x)    | (x, x*x)                | `>[-]<[[>+<]]`                   |
| z = x * y     | (x, y) | (x, y, x*y)             | `>>[-]<<[>[>+<]<]`               |
| swap(x, y)    | (x, y) | (y, x, y)               | `>>[-]<<[>>+<<-]>[<+>-]>[<+>]<<` |
| zero          | (x)    | (0, ..., 0), i=i+x      | `[[-]>[-]]`                      |
| index         | (x)    | (0, ..., i), i=i+x      | `[[-]<[>+<]>+>]`                 |
| goto head     | ()     | i=0                     | `i[<]`                           |
| goto tail     | ()     | i=#-1                   | `#-[>]`                          |
| jump left     | (x)    | (x, #-x), i=x           | `>i<[>-<]>[<]`                   |
| jump right    | (x)    | (x-i, i), i=x           | `>i-[<->]<[>]`                   |
| index n cells | (x)    | (i, ..., i+x-1), i+=x-1 | `-[i>]i`                         |

## Constants

The following tables list the fewest possible single-element operations for various numbers:

<details>
<summary>8-bit (0, 1, 2, 3, ..., 255)</summary>

| Dec | Hex  | Operations         | Length |
|----:|:----:|:-------------------|-------:|
|   0 | 0x00 |                    |      0 |
|   1 | 0x01 | `+`                |      1 |
|   2 | 0x02 | `++`               |      2 |
|   3 | 0x03 | `+++`              |      3 |
|   4 | 0x04 | `++++`             |      4 |
|   5 | 0x05 | `+++++`            |      5 |
|   6 | 0x06 | `++++++`           |      6 |
|   7 | 0x07 | `+++++++`          |      7 |
|   8 | 0x08 | `++++[+]`          |      7 |
|   9 | 0x09 | `+++[++]`          |      7 |
|  10 | 0x0A | `+++++[+]`         |      8 |
|  11 | 0x0B | `++[[+]+]`         |      8 |
|  12 | 0x0C | `++++[++]`         |      8 |
|  13 | 0x0D | `++++[++]+`        |      9 |
|  14 | 0x0E | `++[+[+]]`         |      8 |
|  15 | 0x0F | `+++++[++]`        |      9 |
|  16 | 0x10 | `++++[+++]`        |      9 |
|  17 | 0x11 | `+++[[+]-]`        |      9 |
|  18 | 0x12 | `++[[++]]`         |      8 |
|  19 | 0x13 | `++[[++]]+`        |      9 |
|  20 | 0x14 | `++[++[+]]`        |      9 |
|  21 | 0x15 | `++[++[+]]+`       |     10 |
|  22 | 0x16 | `++[[++]+]`        |      9 |
|  23 | 0x17 | `+++[[+]]-`        |      9 |
|  24 | 0x18 | `+++[[+]]`         |      8 |
|  25 | 0x19 | `+++[[+]]+`        |      9 |
|  26 | 0x1A | `+++[[+]]++`       |     10 |
|  27 | 0x1B | `++[[+++]-]`       |     10 |
|  28 | 0x1C | `++[+[++]]--`      |     11 |
|  29 | 0x1D | `++[+[++]]-`       |     10 |
|  30 | 0x1E | `++[+[++]]`        |      9 |
|  31 | 0x1F | `+++[[+]+]`        |      9 |
|  32 | 0x20 | `++[[+++]]`        |      9 |
|  33 | 0x21 | `++[[+++]]+`       |     10 |
|  34 | 0x22 | `++++[-[+]]`       |     10 |
|  35 | 0x23 | `++++[-[+]]+`      |     11 |
|  36 | 0x24 | `+++[+[+]]--`      |     11 |
|  37 | 0x25 | `+++[+[+]]-`       |     10 |
|  38 | 0x26 | `+++[+[+]]`        |      9 |
|  39 | 0x27 | `+++[+[+]]+`       |     10 |
|  40 | 0x28 | `+++[+[+]]++`      |     11 |
|  41 | 0x29 | `+++[-[++]]-`      |     11 |
|  42 | 0x2A | `+++[-[++]]`       |     10 |
|  43 | 0x2B | `+++[-[++]]+`      |     11 |
|  44 | 0x2C | `+++[+[+]+]-`      |     11 |
|  45 | 0x2D | `+++[+[+]+]`       |     10 |
|  46 | 0x2E | `+++[+[+]+]+`      |     11 |
|  47 | 0x2F | `++[+[+++]-]`      |     11 |
|  48 | 0x30 | `++++[[+]-]-`      |     11 |
|  49 | 0x31 | `++++[[+]-]`       |     10 |
|  50 | 0x32 | `++[[++++]]`       |     10 |
|  51 | 0x33 | `+++[++[+]]-`      |     11 |
|  52 | 0x34 | `+++[++[+]]`       |     10 |
|  53 | 0x35 | `+++[++[+]]+`      |     11 |
|  54 | 0x36 | `++[+++[++]]`      |     11 |
|  55 | 0x37 | `+++[-[++]+]`      |     11 |
|  56 | 0x38 | `++[[++++]+]`      |     11 |
|  57 | 0x39 | `++[+[+++]+]`      |     11 |
|  58 | 0x3A | `+++[++[+]+]-`     |     12 |
|  59 | 0x3B | `+++[++[+]+]`      |     11 |
|  60 | 0x3C | `+++[++[+]+]+`     |     12 |
|  61 | 0x3D | `++++[[+]]---`     |     12 |
|  62 | 0x3E | `++++[[+]]--`      |     11 |
|  63 | 0x3F | `++++[[+]]-`       |     10 |
|  64 | 0x40 | `++++[[+]]`        |      9 |
|  65 | 0x41 | `++++[[+]]+`       |     10 |
|  66 | 0x42 | `++++[[+]]++`      |     11 |
|  67 | 0x43 | `+++[[++]-]-`      |     11 |
|  68 | 0x44 | `+++[[++]-]`       |     10 |
|  69 | 0x45 | `+++[[++]-]+`      |     11 |
|  70 | 0x46 | `+++[[++]-]++`     |     12 |
|  71 | 0x47 | `++[++[+++]]-`     |     12 |
|  72 | 0x48 | `++[++[+++]]`      |     11 |
|  73 | 0x49 | `+++[+++[+]+]`     |     12 |
|  74 | 0x4A | `++[+[++++]-]`     |     12 |
|  75 | 0x4B | `+++[+[+]][+]-`    |     13 |
|  76 | 0x4C | `+++[+[+]][+]`     |     12 |
|  77 | 0x4D | `++++[[+]+]--`     |     12 |
|  78 | 0x4E | `++++[[+]+]-`      |     11 |
|  79 | 0x4F | `++++[[+]+]`       |     10 |
|  80 | 0x50 | `+++[[++]]-`       |     10 |
|  81 | 0x51 | `+++[[++]]`        |      9 |
|  82 | 0x52 | `+++[[++]]+`       |     10 |
|  83 | 0x53 | `+++[[++]]++`      |     11 |
|  84 | 0x54 | `++++[--[++]]`     |     12 |
|  85 | 0x55 | `++++[--[++]]+`    |     13 |
|  86 | 0x56 | `++[+[++++]+]`     |     12 |
|  87 | 0x57 | `+++[-[+++]-]`     |     12 |
|  88 | 0x58 | `+++[-[+++]-]+`    |     13 |
|  89 | 0x59 | `+++[+[+]+][+]-`   |     14 |
|  90 | 0x5A | `+++[+[+]+][+]`    |     13 |
|  91 | 0x5B | `++++[+[+]]---`    |     13 |
|  92 | 0x5C | `++++[+[+]]--`     |     12 |
|  93 | 0x5D | `++++[+[+]]-`      |     11 |
|  94 | 0x5E | `++++[+[+]]`       |     10 |
|  95 | 0x5F | `++++[+[+]]+`      |     11 |
|  96 | 0x60 | `++++[+[+]]++`     |     12 |
|  97 | 0x61 | `+++++[-[+]]-`     |     12 |
|  98 | 0x62 | `+++++[-[+]]`      |     11 |
|  99 | 0x63 | `+++++[-[+]]+`     |     12 |
| 100 | 0x64 | `+++++[-[+]]++`    |     13 |
| 101 | 0x65 | `++[[-[+]]+++]`    |     13 |
| 102 | 0x66 | `++++[-[+]][++]`   |     14 |
| 103 | 0x67 | `+++[++[+]][+]-`   |     14 |
| 104 | 0x68 | `+++[++[+]][+]`    |     13 |
| 105 | 0x69 | `+++[+[++]-]--`    |     13 |
| 106 | 0x6A | `+++[+[++]-]-`     |     12 |
| 107 | 0x6B | `+++[+[++]-]`      |     11 |
| 108 | 0x6C | `+++[-[+++]]`      |     11 |
| 109 | 0x6D | `++++[+[+]+]`      |     11 |
| 110 | 0x6E | `++++[+[+]+]+`     |     12 |
| 111 | 0x6F | `++++[+[+]+]++`    |     13 |
| 112 | 0x70 | `++[++++[+++]]`    |     13 |
| 113 | 0x71 | `++[+[+++++]]-`    |     13 |
| 114 | 0x72 | `++[+[+++++]]`     |     12 |
| 115 | 0x73 | `++[+[+++++]]+`    |     13 |
| 116 | 0x74 | `++[++[++++]+]`    |     13 |
| 117 | 0x75 | `+++[+[++]]---`    |     13 |
| 118 | 0x76 | `+++[+[++]]--`     |     12 |
| 119 | 0x77 | `+++[+[++]]-`      |     11 |
| 120 | 0x78 | `+++[+[++]]`       |     10 |
| 121 | 0x79 | `+++[+[++]]+`      |     11 |
| 122 | 0x7A | `+++[+[++]]++`     |     12 |
| 123 | 0x7B | `++++[++[+]]-`     |     12 |
| 124 | 0x7C | `++++[++[+]]`      |     11 |
| 125 | 0x7D | `++++[++[+]]+`     |     12 |
| 126 | 0x7E | `++++[++[+]]++`    |     13 |
| 127 | 0x7F | `++[[[+]-]]--`     |     12 |
| 128 | 0x80 | `++[[[+]-]]-`      |     11 |
| 129 | 0x81 | `++[[[+]-]]`       |     10 |
| 130 | 0x82 | `++[[[+]-]]+`      |     11 |
| 131 | 0x83 | `++[[[+]-]]++`     |     12 |
| 132 | 0x84 | `+++[+[++]+]-`     |     12 |
| 133 | 0x85 | `+++[+[++]+]`      |     11 |
| 134 | 0x86 | `+++[+[++]+]+`     |     12 |
| 135 | 0x87 | `+++[+[++]+]++`    |     13 |
| 136 | 0x88 | `+++[[++]-][+]`    |     13 |
| 137 | 0x89 | `++[[+++][+]+]`    |     13 |
| 138 | 0x8A | `++++[++[+]+]-`    |     13 |
| 139 | 0x8B | `++++[++[+]+]`     |     12 |
| 140 | 0x8C | `++++[++[+]+]+`    |     13 |
| 141 | 0x8D | `++++[++[+]+]++`   |     14 |
| 142 | 0x8E | `++[++[++]-[+]]`   |     14 |
| 143 | 0x8F | `++[++[++]-[+]]+`  |     15 |
| 144 | 0x90 | `+++[++[++]-]--`   |     14 |
| 145 | 0x91 | `+++[++[++]-]-`    |     13 |
| 146 | 0x92 | `+++[++[++]-]`     |     12 |
| 147 | 0x93 | `+++[++[++]-]+`    |     13 |
| 148 | 0x94 | `+++[++[++]-]++`   |     14 |
| 149 | 0x95 | `+++[[+++]--]-`    |     13 |
| 150 | 0x96 | `+++[[+++]--]`     |     12 |
| 151 | 0x97 | `+++[[+++]--]+`    |     13 |
| 152 | 0x98 | `++[[++][++]-]`    |     13 |
| 153 | 0x99 | `++++[+++[+]]-`    |     13 |
| 154 | 0x9A | `++++[+++[+]]`     |     12 |
| 155 | 0x9B | `++++[+++[+]]+`    |     13 |
| 156 | 0x9C | `++[++[+++++]]`    |     13 |
| 157 | 0x9D | `++[[[+]]---]`     |     12 |
| 158 | 0x9E | `+++++[[+]]--`     |     12 |
| 159 | 0x9F | `+++++[[+]]-`      |     11 |
| 160 | 0xA0 | `+++++[[+]]`       |     10 |
| 161 | 0xA1 | `+++++[[+]]+`      |     11 |
| 162 | 0xA2 | `+++++[[+]]++`     |     12 |
| 163 | 0xA3 | `+++++[[+]]+++`    |     13 |
| 164 | 0xA4 | `++++[-[++]-]`     |     12 |
| 165 | 0xA5 | `++++[-[++]-]+`    |     13 |
| 166 | 0xA6 | `++++[-[++]-]++`   |     14 |
| 167 | 0xA7 | `++++[-[++]-]+++`  |     15 |
| 168 | 0xA8 | `++++[+++[+]+]-`   |     14 |
| 169 | 0xA9 | `++++[+++[+]+]`    |     13 |
| 170 | 0xAA | `+++[[+++]-]-`     |     12 |
| 171 | 0xAB | `+++[[+++]-]`      |     11 |
| 172 | 0xAC | `+++[++[++]+]`     |     12 |
| 173 | 0xAD | `+++[++[++]+]+`    |     13 |
| 174 | 0xAE | `+++[++[++]+]++`   |     14 |
| 175 | 0xAF | `+++[++[++]+]+++`  |     15 |
| 176 | 0xB0 | `++[++[+]+[++]]-`  |     15 |
| 177 | 0xB1 | `++[++[+]+[++]]`   |     14 |
| 178 | 0xB2 | `++[[++++]-[+]]`   |     14 |
| 179 | 0xB3 | `++[[++++]-[+]]+`  |     15 |
| 180 | 0xB4 | `+++[+[+]+][+++]`  |     15 |
| 181 | 0xB5 | `++[+[+++]-[+]]-`  |     15 |
| 182 | 0xB6 | `++[+[+++]-[+]]`   |     14 |
| 183 | 0xB7 | `++++[++++[+]]-`   |     14 |
| 184 | 0xB8 | `++++[++++[+]]`    |     13 |
| 185 | 0xB9 | `+++[+++[++]-]`    |     13 |
| 186 | 0xBA | `++++[+[+]]-[+]`   |     14 |
| 187 | 0xBB | `++++[+[+]][+]-`   |     14 |
| 188 | 0xBC | `++++[+[+]][+]`    |     13 |
| 189 | 0xBD | `+++++[[+]+]--`    |     13 |
| 190 | 0xBE | `+++++[[+]+]-`     |     12 |
| 191 | 0xBF | `+++++[[+]+]`      |     11 |
| 192 | 0xC0 | `+++[[+++]]`       |     10 |
| 193 | 0xC1 | `+++[[+++]]+`      |     11 |
| 194 | 0xC2 | `+++[[+++]]++`     |     12 |
| 195 | 0xC3 | `++++++[-[+]-]`    |     13 |
| 196 | 0xC4 | `++++++[-[+]-]+`   |     14 |
| 197 | 0xC5 | `+++[+++[++]]-`    |     13 |
| 198 | 0xC6 | `+++[+++[++]]`     |     12 |
| 199 | 0xC7 | `+++[+++[++]]+`    |     13 |
| 200 | 0xC8 | `++[+[+++][+]]`    |     13 |
| 201 | 0xC9 | `++++[-[++]]---`   |     14 |
| 202 | 0xCA | `++++[-[++]]--`    |     13 |
| 203 | 0xCB | `++++[-[++]]-`     |     12 |
| 204 | 0xCC | `++++[-[++]]`      |     11 |
| 205 | 0xCD | `++++[-[++]]+`     |     12 |
| 206 | 0xCE | `++++[-[++]]++`    |     13 |
| 207 | 0xCF | `++++[-[++]]+++`   |     14 |
| 208 | 0xD0 | `++++[-[++]]++++`  |     15 |
| 209 | 0xD1 | `++[+[+++][+]+]`   |     14 |
| 210 | 0xD2 | `+++[+++[++]+]-`   |     14 |
| 211 | 0xD3 | `+++[+++[++]+]`    |     13 |
| 212 | 0xD4 | `+++[[+++]+]-`     |     12 |
| 213 | 0xD5 | `+++[[+++]+]`      |     11 |
| 214 | 0xD6 | `+++[[+++]+]+`     |     12 |
| 215 | 0xD7 | `+++[[+++]+]++`    |     13 |
| 216 | 0xD8 | `+++[-[+++]][+]`   |     14 |
| 217 | 0xD9 | `+++[-[+++]][+]+`  |     15 |
| 218 | 0xDA | `++++[+[+]+][+]`   |     14 |
| 219 | 0xDB | `+++[-[++++]]-`    |     13 |
| 220 | 0xDC | `+++[-[++++]]`     |     12 |
| 221 | 0xDD | `+++++[+[+]]-`     |     12 |
| 222 | 0xDE | `+++++[+[+]]`      |     11 |
| 223 | 0xDF | `+++++[+[+]]+`     |     12 |
| 224 | 0xE0 | `+++++[+[+]]++`    |     13 |
| 225 | 0xE1 | `+++++[+[+]]+++`   |     14 |
| 226 | 0xE2 | `+++++[+[+]]++++`  |     15 |
| 227 | 0xE3 | `++[+[+++]+[+]+]`  |     15 |
| 228 | 0xE4 | `++[+[+++++]][+]`  |     15 |
| 229 | 0xE5 | `++++[+++++[+]+]`  |     15 |
| 230 | 0xE6 | `++++[+++++[+]+]+` |     16 |
| 231 | 0xE7 | `+++[[+++]++]---`  |     15 |
| 232 | 0xE8 | `+++[[+++]++]--`   |     14 |
| 233 | 0xE9 | `+++[[+++]++]-`    |     13 |
| 234 | 0xEA | `+++[[+++]++]`     |     12 |
| 235 | 0xEB | `+++[[+++]++]+`    |     13 |
| 236 | 0xEC | `+++[++++[++]]-`   |     14 |
| 237 | 0xED | `+++[++++[++]]`    |     13 |
| 238 | 0xEE | `+++[++++[++]]+`   |     14 |
| 239 | 0xEF | `+++[+[++]][+]-`   |     14 |
| 240 | 0xF0 | `+++[+[++]][+]`    |     13 |
| 241 | 0xF1 | `+++[+[++]][+]+`   |     14 |
| 242 | 0xF2 | `++++[-[++]+]--`   |     14 |
| 243 | 0xF3 | `++++[-[++]+]-`    |     13 |
| 244 | 0xF4 | `++++[-[++]+]`     |     12 |
| 245 | 0xF5 | `++++[-[++]+]+`    |     13 |
| 246 | 0xF6 | `++++[-[++]+]++`   |     14 |
| 247 | 0xF7 | `++++[-[++]+]+++`  |     15 |
| 248 | 0xF8 | `++++[++[+]][+]`   |     14 |
| 249 | 0xF9 | `++[[+++]-[++]]`   |     14 |
| 250 | 0xFA | `+++[++++[++]+]`   |     14 |
| 251 | 0xFB | `+++[-[++++]+]`    |     13 |
| 252 | 0xFC | `+++++[+[+]+]-`    |     13 |
| 253 | 0xFD | `+++++[+[+]+]`     |     12 |
| 254 | 0xFE | `+++++[+[+]+]+`    |     13 |
| 255 | 0xFF | `+++[+[+++]-]`     |     12 |

</details>

## Tools

### nterpreter

*nterpreter* is an ![(**N**)](figures/n.svg) interpreter. The usage of *nterpreter* is as follows:

```.sh
n <input file> [options] [first element] [second element] ... [nth element]
```

#### Options:

* `--binary, -b`: Write output sequence as a series of bytes.
* `--output, -o <file>`: Write output sequence to a file.

### bin2n

*bin2n* is a tool which can convert any binary file into an ![(**N**)](figures/n.svg) program which, when executed, will reproduce the exact sequence of bytes which made up the binary file. The usage of *bin2n* is as follows:

```.sh
bin2n <input file> [output file]
```

## License

Copyright © 2020 Christopher J. Howard. This text is licensed under the [Creative Commons Attribution-ShareAlike 4.0 International License](https://creativecommons.org/licenses/by-sa/4.0/). nterpreter and bin2n source codes are licensed under the [GNU General Public License, version 3](./LICENSE.md).
