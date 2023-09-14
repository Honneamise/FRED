ORG     0x1000      ;set entry point

MAIN:
        LDI     0xFF        ; store 0xFF in D
        PHI     R1          ; store D in high byte of R1
        PLO     R1          ; store D in low byte of R1
        SEX     R1          ; make R1 our stack pointer

SOUND_ON:
        BN1     SOUND_ON    ; wait until signal on EF1
        
        INP     1           ; M(R(X)) <- BUS; D <- BUS

        LDI     0x03        ; load the the note to play

        STR     R1          ; M(R(X)) <- D;

        OUT     3           ; BUS <- M(R(X)) ; R(X) <- R(X)+1
        DEC     R1          ; R1--
        
SOUND_OFF:

        BN1     SOUND_OFF   ; wait until signal on EF1

        INP     1           ; M(R(X)) <- BUS; D <- BUS

        LDI     0x00        ; load 0x00 to stop audio playback

        STR     R1          ; M(R(X)) <- D;

        OUT     3           ; BUS <- M(R(X)) ; R(X) <- R(X)+1
        DEC     R1          ; R1--

        BR      SOUND_ON    ; repeat

END:    IDL                 ; never reached