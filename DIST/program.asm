ORG     0x1000      ; set entry point

MAIN:
        SEX     R1          ; use R1 as video memory pointer

LOOP:                       ; make R1 point to last video memory byte

        LDI     0x07        ; load 0x07
        PHI     R1          ; store it in high byte of R1
        LDI     0xCF        ; load 0xCF
        PLO     R1          ; store it in low byte of R1         
        
        INC     R3          ; increase attribute color

SCREEN:
        GLO     R3          ; get color attribute
        STXD                ; M(R(X)) <- D; R(X) <- R(X)-1

        LDI     0xB1        ; set character attribute
        STXD                ; M(R(X)) <- D; R(X) <- R(X)-1

                            ; is high byte of R1 equal to 0xFF ?
        GHI     R1          ; get high byte of R1
        XRI     0xFF        ; xor with 0xFF
        BNZ     SCREEN      ; if not zero branch

                            ; is low byte of R1 equal to 0xFF ?
        GLO     R1          ; get low byte of R1
        XRI     0xFF        ; xor with 0xFF
        BNZ     SCREEN      ; if not zero branch

WAIT_KEY:
        BN1     WAIT_KEY    ; wait until signal on EF1

EMPTY:
        INP     1           ; M(R(X)) <- BUS; D <- BUS

        B1      EMPTY       ; empty the keyboard buffer

        BR      LOOP        ; repeat

END:    IDL                 ; never reached