# 1802 LIBRARY

The library is composed of two modules :

- LIB_STD : __"Standard Subroutine Call and Return Technique"__

- LIB_ROUTINES : __"Binary Arithmetic Subroutines for RCA COSMAC Microprocessors"__

Both are from the original documentation of 1976.

To correctly setup the registers as required see the example provided later.

For utilization and implementation details see [1802 LIBRARY](1802_LIBRARY.PDF).

## LIB_STD

Including the file __"lib_std.asm"__ allow the access to the following functions :

- STANDARD
    - CALL
    - RETURN

## LIB_ROUTINES

Including the file __"lib_routines.asm"__ allow the access to the following functions :

- ADDITION
    - ADDOP
    - ADD
    - ADDCON
    - ADDST
- SUBTRACTION
    - SDOP
    - SD
    - SDCON
    - SMOP
    - SM
- MULTIPLICATION
    - MPYOP
    - MPY
- DIVISION
    - DIVOP
    - DIV0
    - DIV
    - DIVQ
- BCD CONVERSIONS
    - CBD
    - CDB
- UTILITIES
    - PUSHAC
    - PUSHCQ
    - POPAC
    - POP
    - POPCQ
    - LOADOP
    - LOAD
    - LODCON
    - STOROP
    - STORE
    - COMPOP
    - COMP
    - TEST
    - SWAPAQ

## LIBRARY TEST PROGRAM

```
;WORD LABEL is the equivalent of ,A(LABEL) in the original notation 

ORG 0x1000                  ; entry point
    
    LOAD_REG    R2, 0xFFFF  ; setup stack pointer to 0xFFFF
    LOAD_REG    R3, MAIN    ; set program counter to MAIN label
    LOAD_REG    R4, CALLS   ; set register for CALL
    LOAD_REG    R5, RET     ; set register for RETURN
    SEP         R3          ; set new program counter to MAIN

    INC_SRC "lib_std.asm"       ; include CALL/RET utilities
    RESERVE 0xCA                ; library need page alignment
    INC_SRC "lib_routines.asm"  ; include library functions

MAIN:
    LOAD_REG    MQ, 0x0007  ; clear MQ
    LOAD_REG    AC, 0xA121  ; load AC
    LOAD_REG    MA, VAL

    SEP         CALL        ; prepare call
    WORD        DIVQ          ; call to routine address 
    ;WORD        VAL

BYTE 0x68            ; dump the cpu status

    IDL                     ; halt

VAL:
    WORD 0x0002
```

## NOTES ABOUT TRANSCRIPTION

The transcription of the original library tries to be as faithful as possible, but there are some basic differences:

- numbers are always expressed with their hexadecimal value (0x)
- the arithmetic expressions have been replaced with new labels (_XX)
- the value 0x38 has been replaced with the corresponding SKP instruction

__Details :__

- Line 194 : BR  _01(205)
- Line 222 : BNZ _02(226)
- Line 273 : BDF _03(280)
- Line 279 : SKP
- Line 349 : BNZ _04(352)
- Line 361 : BDF _05(330)
- Line 372 : BDF _06(375)
- Line 408 : BDF _07(410)
- Line 432 : BDF _08(435)
- Line 439 : SKP
- Line 451 : BR  _09(457)
- Line 459 : BNZ _10(462)
- Line 515 : BNF _11(519)
- Line 717 : SKP
- Line 731 : BNZ _12(725)
- Line 733 : BZ  _13(727)
- Line 734 : BR  _12(725)
- Line 749 : BNF _14(755)
- Line 806 : BDF _15(834)
- Line 815 : BDF _15(834)
- Line 824 : BDF _16(835)
- Line 845 : BDF _17(844)
- Line 891 : BZ  _18(896)
- Line 894 : BR  _19(888)
- Line 908 : BNZ NEXT(910)
- Line 914 : BNF _20(916)
- Line 930 : BR  _18(896)