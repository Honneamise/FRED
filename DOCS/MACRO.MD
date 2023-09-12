# MACRO

## ORG

Define the entry point of the user's program, must be set to **0x1000**

```
ORG 0x1000
```

## .1 and .0

Select the high byte ( **.1** ) or the low byte ( **.0** ) of a 16 bit address value

```
;store in R5 the address of LABEL

LDI    LABEL.1     
PHI    R5
LDI    LABEL.0
PLO    R5

LABEL: 
```

## BYTE

Store one or more byte as binary data, for multiple values use the comma as separator.

```
BYTE 0x01

BYTE 0x01,0x02,0x03
```

## STRING

Store a sequence of ascii characters

```
STRING "HELLO WORLD"
```

## INCLUDE_SRC

Include the contents of a file as source code

```
INCLUDE_SRC "file.asm"
```

## INCLUDE_BIN

Include the contents of a file as binary data

```
INCLUDE_SRC "file.bin"
```
