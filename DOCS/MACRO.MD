# MACRO

## ORG

Define the entry point of the user's program, must be set to **0x1000**

```
ORG 0x1000
```

## .1 and .0

Select the high byte ( **.1** ) or the low byte ( **.0** ) of a 16 bit address value

```
LDI    LABEL.1     
PHI    R5
LDI    LABEL.0
PLO    R5
```

## BYTE

Store one or more byte as binary data, for multiple values use the comma as separator.

```
BYTE 0x01

BYTE 0x01,0x02,0x03

BYTE LABEL.1
```

## WORD

Store one or more word as binary data, for multiple values use the comma as separator.

```
WORD 0x1234

WORD 0x1234, 0x5678

WORD 0xFF ; will store the word [00][FF]

WORD LABEL ; will store the address value of LABEL
```

## RESERVE

Reserve a given number of bytes using 0x00 as filler.

```
RESERVE 123
```

## STRING

Store a sequence of ascii characters

```
STRING "HELLO WORLD"
```

## INC_SRC

Include the contents of a file as source code

```
INC_SRC "file.asm"
```

## INC_BIN

Include the contents of a file as binary data

```
INC_BIN "file.bin"
```

## LOAD_REG

Load a register with a value taken from a number, a register or a label.

```
LOAD_REG R1,0x69    ; will store 0x0069 in R1

LOAD_REG R1,0x1234  ; will store 0x1234 in R1

LOAD_REG R1,R2      ; will store the value of R2 in R1

LOAD_REG R1,LABEL   ; will store the address value of LABEL in R1
```
