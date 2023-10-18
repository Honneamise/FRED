ORG 0x1000

    LOAD_REG    SP, 0xFFFF      ; setup stack pointer to 0xFFFF
    SEX         SP              ; set SP also to X
    LOAD_REG    PC, MAIN        ; set program counter to MAIN label
    SEP         PC              ; set the new program counter

MAIN:
    LDI         0x01        ; set graph mode (0x01)
    STR         SP          ; M(SP) <-- D
    OUT         1           ; BUS <- M(R(X)) ; R(X) <- R(X)+1
    DEC         R1          ; decrease stack pointer

IMAGE_LOAD:
    LOAD_REG    R7, IMAGE   ; src pointer
    LOAD_REG    R8, 0x0000  ; dst pointer
    LOAD_REG    R9, 0x0000  ; counter

IMAGE_LOOP:                 ; will stop when R9 is 0x07D0(2000)
    LDA         R7          ; load data and advance
    XRI         0xFF        ; bits in pbm images are inverted
    STR         R8          ; store
    INC         R8          ; increase store pointer
    
    GHI         R9          ; get high byte of counter
    XRI         0x07        ; xor with 0x07
    BNZ         IMAGE_LOOP  ; if not zero repeat

    GLO         R9          ; get low byte of counter
    XRI         0xD0        ; xor with 0xD0
    BNZ         IMAGE_LOOP  ; if not zero repeat

FINISH: BR      FINISH


IMAGE: INC_BIN "image.bin"