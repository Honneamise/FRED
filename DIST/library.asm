;==============================================================================
; LIBRARY
;==============================================================================
CALLR:  SEP R3
CALLS:  SEX R2
        GHI R6
        STXD
        GLO R6
        STXD
        GHI R3
        PHI R6
        GLO R3
        PLO R6
        LDA R6
        PHI R3
        LDA R6
        PLO R3
        BR CALLR

RETR:   SEP R3
RETS:   GHI R6
        PHI R3
        GLO R6
        PLO R3
        SEX R2
        INC R2
        LDXA
        PLO R6
        LDX
        PHI R6
        BR RETR

SWAPQ:  GHI MQ
        PLO CR
        GHI AC
        PHI MQ
        GLO MQ
        PHI AC
        GLO AC
        PLO MQ
        GHI AC
        PLO AC
        GLO CR
        PHI AC
        SEP RETN

PUSHAC: GHI SP
        PHI MA
        GLO SP
        PLO MA
        INC MA
        INC MA
        SEX SP
        LDN MA
        STXD
        DEC MA
        LDN MA
        STXD
        GHI AC
        STR MA
        GLO AC
        INC MA
        STR MA
        DEC MA
        SEP RETN
