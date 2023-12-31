# 1802 LIBRARY

The library is composed of two modules :

- LIB_STD : __"Standard Subroutine Call and Return Technique"__

- LIB_ROUTINES : __"Binary Arithmetic Subroutines for RCA COSMAC Microprocessors"__

Both are from the original documentation of 1976.

To correctly setup the registers as required see the example provided later.

For utilization and implementation details see [1802 LIBRARY](1802_LIBRARY.PDF).

## LIB_STD

Including the file [lib_std.asm](lib_std.asm) allow the access to the following functions :

- STANDARD
    - CALL
    - RETURN

## LIB_ROUTINES

Including the file [lib_routines.asm](lib_routines.asm) allow the access to the following functions :

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

Simple test program for library ( assembly source file : [lib_test.asm](lib_test.asm) )

Usage from the __"DIST"__ folder :

- to assemble : __./FRED_ASM.EXE ../LIBRARY/lib_test.asm ../LIBRARY/lib_test.bin__

- to run : __./FRED_EMU.EXE 1 charset.rom ../LIBRARY/lib_test.bin__


```
    ;-----------------------------------------------------------------------
    ; Test program for "Standard Subroutine Call and Return Technique" 
    ; and "Binary Arithmetic Subroutines for RCA COSMAC Microprocessors"
    ;
    ; NOTES:
    ; - tests are taken from the 1976 manual where an example is available  
    ; - WORD LABEL is the equivalent of ,A(LABEL) in the original notation 
    ;-----------------------------------------------------------------------

ORG 0x1000                      ; entry point
    
    LOAD_REG    CALL, CALLS     ; set register for CALL
    LOAD_REG    RETN, RET       ; set register for RETURN
    LOAD_REG    SP, 0xFFFF      ; setup stack pointer to 0xFFFF
    SEX         SP              ; set SP as X
    LOAD_REG    PC, MAIN        ; set program counter to MAIN label
    SEP         PC              ; set PC as P

    INC_SRC "../LIBRARY/lib_std.asm"        ; include CALL/RET utilities
    RESERVE 0xCA                            ; library need page alignment(!!!)
    INC_SRC "../LIBRARY/lib_routines.asm"   ; include library functions

MAIN:
    ;-----------------------------------------------------------------------
    ; ADDOP
    ;-----------------------------------------------------------------------
    LOAD_REG    AC, 0xFFFE      ; load 0xFFFE(-2) in AC
    SEP         CALL            ; prepare call
    WORD        ADDOP           ; routine address
    WORD        OPR1            ; operand address
    BYTE        0x68            ; DUMP, expect AC=FFFF(-1); DF=0

    ;-----------------------------------------------------------------------
    ; SDOP
    ;-----------------------------------------------------------------------
    LOAD_REG    AC, 0x000A      ; load 0x000A in AC
    SEP         CALL            ; prepare call
    WORD        SDOP            ; routine address
    WORD        MINU            ; operand address
    BYTE        0x68            ; DUMP, expect AC=FFFB(-5); DF=0

    ;-----------------------------------------------------------------------
    ; SDCON
    ;-----------------------------------------------------------------------
    LOAD_REG    AC, 0x0002      ; load 0x0002 in AC
    SEP         CALL            ; prepare call
    WORD        SDCON           ; routine address
    WORD        0x0005          ; constant
    BYTE        0x68            ; DUMP, expect AC=0x0003; DF=0

    ;-----------------------------------------------------------------------
    ; SM
    ;-----------------------------------------------------------------------
    LOAD_REG    AC, 0x8000      ; load 0x8000 in AC
    LOAD_REG    MA, SUBT        ; load operand address in MA
    SEP         CALL            ; prepare call
    WORD        SM              ; routine address
    BYTE        0x68            ; DUMP, expect DF=1

    ;-----------------------------------------------------------------------
    ; MPYOP
    ;-----------------------------------------------------------------------
    LOAD_REG    AC, 0xFFFF      ; load 0xFFFF(-1) in AC
    LOAD_REG    MA, MPLR        ; load operand address in MA
    SEP         CALL            ; prepare call
    WORD        MPYOP           ; routine address
    WORD        MPLR            ; operand address
    BYTE        0x68            ; DUMP, expect MQ=0xFFFF; AC=0xFFFF, DF=0

    ;-----------------------------------------------------------------------
    ; DIVOP 1
    ; WARNING   : this test will pass without modification
    ;           : default order divisions order is : DIVOP,DIV0,DIV,DIVQ
    ;           : required order is : DIVOP,DIV0,DIVQ
    ;           : please apply required modifications in lib_routines.asm
    ;-----------------------------------------------------------------------
    LOAD_REG    AC, 0x000A      ; load 0x000A in AC
    SEP         CALL            ; prepare call
    WORD        DIVOP           ; routine address
    WORD        DIVR            ; operand address
    BYTE        0x68            ; DUMP, expect MQ=0x0001; AC=0x0003, DF=0

    ;-----------------------------------------------------------------------
    ; DIVOP 2
    ; WARNING   : without modification this test will fail
    ;           : default order divisions order is : DIVOP,DIV0,DIV,DIVQ
    ;           : required order is : DIVOP,DIV,DIVQ 
    ;           : please apply required modifications in lib_routines.asm
    ;-----------------------------------------------------------------------
                                ; [MQ:AC]=[0x0007:0xa121]=500001
    LOAD_REG    MQ, 0x0007      ; load 0x0007 in MQ
    LOAD_REG    AC, 0xA121      ; load 0xA121 in AC
    SEP         CALL            ; prepare call
    WORD        DIVOP           ; routine address
    WORD        DVSR            ; operand address
    BYTE        0x68            ; DUMP, expect DF=1

    ;-----------------------------------------------------------------------
    ; CBD
    ;-----------------------------------------------------------------------
    LOAD_REG    AC, 0xFFE0      ; load 0xFFE0 in AC
    LOAD_REG    MA, RSLT        ; load operand address in MA
    SEP         CALL            ; prepare call
    WORD        CBD             ; routine address
    WORD        RSLT            ; storing address
    BYTE        0x05            ; size used

                                ; dump memory bytes, chek in D the values 
                                ; sequence : 0x0D, 0x00, 0x00, 0x03, 0x02

    LOAD_REG    R7, 0x0005      ; use R7 as counter 
    LOAD_REG    R8, RSLT        ; load R8 with the address of result
CBD_LOOP:
    LDA         R8              ; D <- M(R(N)); R(N) <- R(N)+1
    BYTE        0x68            ; DUMP, check value in D
    DEC         R7              ; decrease counter
    GLO         R7              ; get low byte of counter
    BNZ         CBD_LOOP        ; if not 0, repeat
CBD_END:
    
    ;-----------------------------------------------------------------------
    ; CDB
    ;-----------------------------------------------------------------------
    LOAD_REG    AC, 0xFFE0      ; load 0xFFE0 in AC
    LOAD_REG    MA, RSLT        ; load operand address in MA
    SEP         CALL            ; prepare call
    WORD        CDB             ; routine address
    WORD        BCD             ; number address
    BYTE        0x05            ; size used
    BYTE        0x68            ; DUMP, expect AC=0x0600

    ;-----------------------------------------------------------------------
    ; LOADOP
    ;-----------------------------------------------------------------------
    SEP         CALL            ; prepare call
    WORD        LOADOP          ; routine address
    WORD        OPR2            ; address of operand
    BYTE        0x68            ; DUMP, expect AC=0xABCD

    ;-----------------------------------------------------------------------
    ; COMPOP
    ;-----------------------------------------------------------------------
    LOAD_REG    AC, 0x0001      ; load 0x0001 in AC
    SEP         CALL            ; prepare call
    WORD        COMPOP          ; routine address
    WORD        OPR3            ; address of operand
    BYTE        0x68            ; DUMP, expect DF=1 0x0001>0xFFFF(-1)

FINISH: BR      FINISH          ; HALT PROGRAM

    ;-----------------------------------------------------------------------
    ; LIST OFOPERANDS
    ;-----------------------------------------------------------------------
OPR1:   WORD 0x0001
MINU:   WORD 0x0005
SUBT:   WORD 0x0001
MPLR:   WORD 0x0001
DIVR:   WORD 0x0003
DVSR:   WORD 0x0002
RSLT:   RESERVE 0x05
BCD:    BYTE 0x0B, 0x01, 0x05, 0x03, 0x06
OPR2:   WORD 0xABCD
OPR3:   WORD 0xFFFF
```

## NOTES ABOUT TRANSCRIPTION

The transcription of the original library tries to be as faithful as possible, but there are some little differences:

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