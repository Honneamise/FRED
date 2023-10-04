# ASSEMBLY REFERENCE

- The programs entry point is located at **0x1000** and must be set using the **ORG** macro.

- The assembler's parser is case-sensitive.

- Labels must terminate with the colon ( **:** ) symbol.

- Labels can contains any sequence of letters, numbers and underscore symbols.

- Comment must start with the semicolon ( **;** ) symbol.

- Only one instruction or macro per line is permitted.

- For debug pourposes the opcode **0x68** will print the cpu info to stdout.

# CODE SNIPPETS

## String print

```
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
```

## Clear the screen

```
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
        BN2     WAIT_KEY    ; wait until signal on EF2

EMPTY:
        INP     2           ; M(R(X)) <- BUS; D <- BUS

        B2      EMPTY       ; empty the keyboard buffer

        BR      LOOP        ; repeat

END:    IDL                 ; never reached
```

## Stack operations

```
        ORG     0x1000      ; set the entry point

MAIN:
        LDI     0xFF        ; store 0xFF in D
        PHI     R1          ; store D in high byte of R1
        PLO     R1          ; store D in low byte of R1
        SEX     R1          ; make R1 our stack pointer

        LDI     0x69        ; store 0x69 in D

        STXD                ; push D on stack

        LDI     0x77        ; store 0x77 in D
   
        IRX                 ; advance stack pointer

        LDX                 ; pop the stack, do not advance

END:    IDL                 ; halt execution
```

## Keyboard input

```
        ORG     0x1000      ; set the entry point

MAIN:
        LDI     0xFF        ; store 0xFF in D
        PHI     R1          ; store D in high byte of R1
        PLO     R1          ; store D in low byte of R1
        SEX     R1          ; make R1 our stack pointer

INPUT:  
        BN2     INPUT       ; wait until signal on EF2

        INP     2           ; M(R(X)) <- BUS; D <- BUS

        STR     R2          ; M(R(X)) <-- D

        INC     R2          ; skip attribute byte      

        INC     R2          ; next pos

        BR      INPUT       ; repeat

END:    IDL                 ; halt execution
```

## Sound on/off

```
        ORG     0x1000      ;set entry point

MAIN:
        LDI     0xFF        ; store 0xFF in D
        PHI     R1          ; store D in high byte of R1
        PLO     R1          ; store D in low byte of R1
        SEX     R1          ; make R1 our stack pointer

SOUND_ON:
        BN2     SOUND_ON    ; wait until signal on EF2
        
        INP     2           ; M(R(X)) <- BUS; D <- BUS

        LDI     0x03        ; load the the note to play

        STR     R1          ; M(R(X)) <- D;

        OUT     4           ; BUS <- M(R(X)) ; R(X) <- R(X)+1
        DEC     R1          ; R1--
        
SOUND_OFF:

        BN2     SOUND_OFF   ; wait until signal on EF1

        INP     2           ; M(R(X)) <- BUS; D <- BUS

        LDI     0x00        ; load 0x00 to stop audio playback

        STR     R1          ; M(R(X)) <- D;

        OUT     4           ; BUS <- M(R(X)) ; R(X) <- R(X)+1
        DEC     R1          ; R1--

        BR      SOUND_ON    ; repeat

END:    IDL                 ; never reached
```

## Set graphic mode
```        
        ; R0 is program counter, R1 is X register

        ORG     0x1000      ; set the entry point
 
MAIN:                       
        SEX     R1          ; R1 is 0x0000 by default

                            ; R2 as string pointer
        LDI     LABEL.1     ; get high byte of LABEL
        PHI     R2          ; store it in high byte of R2
        LDI     LABEL.0     ; get low byte of LABEL
        PLO     R2          ; store it in low byte of R2

PRINT:
        LDA     R2          ; D <- M(R(N)); R(N) <- R(N)+1

        BZ      WAIT_KEY    ; if zero, done

        STR     R1          ; M(R(X)) <-- D
        
        INC     R1          ; skip attribute byte

        INC     R1          ; next pos

        BR     PRINT        ; repeat

WAIT_KEY:
        BN2     WAIT_KEY    ; wait until signal on EF2

SET_MODE:
        LDI     0xFF        ; set R1(sp) to 0xFFFF
        PHI     R1
        PLO     R1

        LDI     0x01        ; GRAPH MODE is 0x01
        STR     R1          ; M(R1) <-- D
        OUT     1           ; BUS <- M(R(X)) ; R(X) <- R(X)+1

        DEC     R1          ; decrease stack pointer

DRAW_PATTERN:
                            ; set R1 to end of video memory
        LDI     0x07        ; load 0x07
        PHI     R1          ; store it in high byte of R1
        LDI     0xCF        ; load 0xCF
        PLO     R1          ; store it in low byte of R1      
                    
SCREEN_LOOP:
        LDI     0xAA        ; set pattern (10101010)

        STXD                ; M(R(X)) <- D; R(X) <- R(X)-1

                            ; is high byte of R1 equal to 0xFF ?
        GHI     R1          ; get high byte of R1
        XRI     0xFF        ; xor with 0xFF
        BNZ     SCREEN_LOOP ; if not zero branch

                            ; is low byte of R1 equal to 0xFF ?
        GLO     R1          ; get low byte of R1
        XRI     0xFF        ; xor with 0xFF
        BNZ     SCREEN_LOOP ; if not zero branch

END:    IDL                 ; halt execution

LABEL: 
        STRING "PRESS A KEY TO SET VIDEO MODE" 
        BYTE 0x00
```