CALLR:  SEP     R3      .. to subroutine
CALLS:  SEX     R2      .. point to stack
        GHI     R6      .. save R6 on stack
        STXD            ..
        GLO     R6      ..
        STXD            ..
        GHI     R3      .. save R3 in R6
        PHI     R6      ..
        GLO     R3      ..
        PLO     R6      ..
        LDA     R6      .. set subroutine address
        PHI     R3      .. in R3
        LDA     R6      ..
        PLO     R3      ..
        BR      CALLR   .. jump to subroutine
                        ..
                        ..
RETR:   SEP     R3      .. return to main
RET:    GHI     R6      .. restore return address
        PHI     R3      .. into R3
        GLO     R6      ..
        PLO     R3      ..
        SEX     R2      .. restore old R6 saved
        INC     R2      .. on stack into R6
        LDXA            ..
        PLO     R6      ..
        LDX             ..
        PHI     R6      ..
        BR      RETR    .. return