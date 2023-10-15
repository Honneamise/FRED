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


