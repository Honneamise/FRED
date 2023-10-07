ORG     0x1000      ; set the entry point
 
MAIN:                       ; R1 as string pointer

        LDI     LABEL.1     ; get high byte of LABEL
        PHI     R1          ; store it in high byte of R1
        LDI     LABEL.0     ; get low byte of LABEL
        PLO     R1          ; store it in low byte of R1

PRINT:
        LDA     R1          ; D <- M(R(N)); R(N) <- R(N)+1

        BZ      END         ; if zero, done

        STR     R2          ; M(R(X)) <-- D
        
        INC     R2          ; skip attribute byte

        INC     R2          ; next pos

        BR     PRINT        ; repeat

END:    IDL                 ; halt execution

LABEL: 
        STRING "HELLO WORLD" 
        BYTE 0x00