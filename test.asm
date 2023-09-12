        ORG     0x1000      ; set the entry point
 
MAIN:                    

        LDI     0xFF        ;R1 stack pointer
        PHI     R1          
        PLO     R1          
        SEX     R1

LOOP:
        LDI     WAIT.1     ;R2 pointer to WAIT_KEY function
        PHI     R2
        LDI     WAIT.0
        PLO     R2

        SEP     R2





WAIT_RET:
        SEP     R0
        
WAIT:
        BN1     WAIT    ; wait until signal on EF1

        INP     1       ; M(R(X)) <- BUS; D <- BUS

        STR     R3

        BR      WAIT_RET
 