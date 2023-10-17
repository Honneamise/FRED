

    .. COPYRIGHT 1976 RCA CORPORATION
    .. COSMAC ARITHMETIC SUBROUTINE PACKAGE
    ..
    .. VERSION 1.1 FOR CDP1802
    ..
    .. EXTRACTED FOR BINARY ARITHMETIC
    .. AND CONVERSIONS FOR DECIMAL
    .. OCCUPIES 1K BYTES
    ..
    ..
    .. DESIGNED FOR STANDARD CSDP SUBROUTINE CALL & RETURN
    ..
    .. FOR STANDARD LINKAGE,
    .. SP= #02 IT SHOULD BE THE STACK POINTER
    .. PC= #03 IT IS THE PROGRAM COUNTER
    .. USED BY THESE SUBROUTINES
    .. CALL= #04 IT SHOULD POINT TO THE ROUTINE
    .. WHICH EFFECTS SUBROUTINE CALLS
    .. RETN= #05 IT SHOULD POINT TO THE ROUTINE
    .. WHICH EFFECT SUBROUTINE RETURN
    .. LINK= #06 IT SHOULD POINT TO CALL PARAMETER
    .. (USUALLY OPERAND ADDRESSES AND/OR CONSTANT)
    ..
    ..
    .. THE FOLLOWING REGISTERS MUST BE ASSIGNED
    .. AR= #0A (USED FOR RESULT ADDRESS)

    .. NR= #0B (USED FOR RESULT DIGIT COUNT)
    .. 16-BIT BINARY ARITHMETIC ROUTINES
    .. THE FOLLOWING REGISTERS MUST BE ASSIGNED
    .. AC= #0F 16-BIT ACCUMULATOR RF
    .. MQ= #0E 16-BIT ACCUMULATOR EXTENSION RE
    .. MA= #0D (TEMPORARY) OPERAND MEMORY ADDRESS
    .. CR= #0C (TEMPORARY) SCRATCHPAD AND COUNTER
    ..
    ..
    .. SWAP AC WITH MQ REGISTERS
SWAPAQ: GHI MQ  .. SAVE MQ.0
    PLO CR      .. IN CR.0 (COULD HAVE PUSHED ON STACK)
    GHI AC      .. NOW AC.1 TO MQ.1
    PHI MQ      ..
    GLO MQ      .. SAVE MQ.0
    PHI AC      .. IN AC.1
    GLO AC      .. THEN AC.0 TO MQ.0
    PLO MQ      ..
    GHI AC      .. NOW SAVED MQ.0 to AC.0
    PLO AC      ..
    GLO CR      .. FINALLY SAVED MQ.1
    PHI AC      .. TO AC.1
    SEP RETN    ..
    .. 16-BIT SUBTRACT AC FROM CONSTANT
    .. **** AC=CONSTANT-AC
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL SDCON; CONSTANT
SDCON: GLO LINK .. (ESSENTIALLY SAME AS ADCON)
    PLO MA      ..
    GHI LINK    ..
    PHI MA      ..
    INC LINK    ..
    INC LINK    ..
    BR  SD      ..
    ..
    .. 16-BIT SUBTRACT AC FROM OPERAND
    .. **** AC=OPRN-AC
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL SDOP; ,A(OPRN)
    ..
SDOP: LDA LINK  .. FETCH OPERAND ADDRESS
    PHI MA      .. TO MA REGISTERS
    LDA LINK    ..
    PLO MA      .. FALL INTO SD  
    .. 16-BIT SUBTRACT AC FROM OPERAND
    .. CALL HERE IF OPERAND
    .. ADDRESS IN REGISTER MA
    .. **** AC=M(R(MA))-AC
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL SD;
    ..
SD: SEX MA      .. SET X PTR TO MA
    GHI AC      .. CHECK SIGN BIT OF AC
    XOR         .. AND OPERAND @MA
    STR SP      .. AND STORE ON STACK
    INC MA      .. POINT TO LOW 8
    GLO AC      .. FETCH AC LOW 8
    SD          .. SUBTRACT FROM LOW 8 IN MEMORY
    PLO AC      .. PUT IT BACK
    DEC MA      .. NOW HIGH 8
    GHI AC      ..
SDNB: SDB       .. SUBTRACT HIGH 8
    PHI AC      .. PUT IT BACK
    LDN SP      .. LOAD STORED COMPARING BIT OF OPERANDS
    SHL         .. CHECK STORED SIGN COMPARISON BIT
    BNF SDFF    .. IF OPERAND'S SIGN ARE SAME
    GHI AC      .. NO OVERFLOW POSSIBLE
    XOR         .. OTHER WISE CHECK RESULT
    SHL         .. SET DF=0 IF OK
SDFF: SEP RETN  .. RETURN
    ..
    .. 16-BIT SUBTRACT FROM AC (ADDRESS IN CALL)
    .. **** AC=AC-OPRN
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL SMOP; ,A(OPRN)
    ..
SMOP: LDA LINK  ..
    PHI MA      ..
    LDA LINK    ..
    PLO MA      ..
    .. 16-BIT SUBTRACT FROM AC (ADDRESS IN MA)
    .. CALL HERE IF OPERAND
    .. ADDRESS IN REGISTER MA
    .. **** AC=AC-M(R(MA))
    .. ************ (TO CALL, WRITE) ************
    .. ****CALL SM;
    ..
SM: SEX MA      .. SET X PTR
    GHI AC      .. GET SIGN OF AC AND
    SHR         .. AND STORE IN 7TH BIT OF CR
    STR SP      .. BUT PUT IN (SP) FIRST
    GHI AC      .. AND SEE IF OPERANDS SIGNS ARE THE SAME
    XOR         ..
    ANI 0x80    .. TAKE OUT COMPARING SIGN BIT
    SEX SP      .. NOW STORE THAT BIT IN 8TH OF CR
    ADD         .. BY ADDING TO IT
    STR SP      .. AND STORE THESE TWO BITS ON THE STACK
    INC MA      .. POINT TO LOW 8
    SEX MA      .. REMEMBER TO SET X OPERANDS 
    GLO AC      .. FETCH AC LOW 8
    SM          .. SUBTRACT MEMORY FROM IT
    PLO AC      .. PUT IT BACK
    DEC MA      .. NOW HIGH 8
    GHI AC      ..
SMNB: SMB       .. HIGH 8 SUBTRACT, NO BORROW ACROSS
    PHI AC      .. PUT HIGH 8 BACK
    SEX SP      .. NOW CHECK IF UNDERFLOWED
    LDX         .. LOAD THE STORED TWO BITS
    SHL         .. AND TAKE OUT THE COMPARING SIGN BIT
    BNF SMRT    .. THE SAME, UNDERFLOW NOT POSSIBLE
    STR SP      .. PUT IT BACK TO STACK
    GHI AC      .. OTHERWISE HAVE TO COMPARE SIGN OF RESULT
    XOR         .. SIGN BIT OF AC WAS STORED ON STACK
    SHL         .. TAKE OUT 7TH BIT
SMRT: SEP RETN  .. DF=0 IF SUBTRACTION OK
    .. 
    ..
    .. 16X16 BIT SIGNED MULTIPLY (2'S COMPLEMENT)
    .. **** AC=AC*OPRN
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL MPYOP; ,A(OPRN)
    ..
MPYOP: LDA LINK .. FETCH MULTIPLICAND ADDRS
    PHI MA      .. INTO REGISTER MA
    LDA LINK    ..
    PLO MA      .. FALL INTO MPY
    .. 16X16 BIT SIGNED MULTIPLY (2'S COMPLEMENT)
    .. CALL HERE IF OPERAND ADDRESS
    .. IN REGISTER MA
    .. **** AC=AC*M(R(MA))
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL MPY;
    ..
MPY: SEX MA     .. SET X NOW
    GHI AC      .. CHECK IF THE SIGN OF MULTIPLICAND
    XOR         .. THE SAME AD SIGN OF MULTIPR
    ANI 0x80    .. AND STORE THAT BIT
    PHI CR      .. INTO CR.1
    LDI 0x10    .. SET COUNTER TO 16
    PLO CR      ..
    LDI 0x00    .. INITIALIZE MQ TO 0
    PHI MQ      .. TO HOLD PRODUCT
    PLO MQ      ..
MPL: DEC CR     .. IF NOT, DECREMENT IT
    GHI AC      .. SHIFT AC (MULTIPLIER) RIGHT
    SHR         ..
    PHI AC      ..
    GLO AC      ..
    SHRC        .. SHIFT 0 ACROSS BYTES
    PLO AC      ..
    GHI MQ      ..
    BNF MPB     .. IF NO BIT OUT, DON'T ADD
    INC MA      .. POINT TO LOW 8 OF MULTIPLICAND
    GLO CR      .. IF NOT LAST ITERATION
    BNZ MPA     .. GO ADD
    GLO MQ      .. LOAD MQ.0 INTO D
    SM          .. MQ.0-(MA.0)
    PLO MQ      .. AND STORE BACK INTO MQ.0
    DEC MA      .. DO THE HIGH BYTE
    GHI MQ      ..
    SMB         .. REMEMBER THE BORROW BIT
    PHI MQ      .. AND PUT BACK INTO MQ.1
    GHI CR      .. NOW ARE THE SIGNS OF OPERANDS THE SAME
MPB: SHL        .. TEST FOR SIGN BIT
    BR  _01     .. IF NEGATIVE, SIGN EXTEND
MPA: GLO MQ     .. DO MQ.0+(MA)
    ADD         .. MQ.0+(MA.0)
    PLO MQ      .. AND PUT BACK INTO MQ.0
    DEC MA      .. SAME FOR HIGH BYTE
    GHI MQ      ..
    ADC         .. ADD WITH CARRY
    PHI MQ      ..
MPS: LSDF       ..
    LDX         ..
    SHL         .. IF OPERAND IS NEGATIVE, THEN
_01: GHI MQ     .. PUT MQ.1 INTO D
    SHRC        .. SHIFT IN CARRY
    PHI MQ      ..
    GLO MQ      .. CONTINUE TO LOW 8 OF MQ    
    SHRC        ..
    PLO MQ      ..
    BNF MT      .. IF NO CARRY OUT, ITERATE
    GHI AC      .. ADD CARRY OUT INTO AC MSB
    ORI 0x80    ..
    PHI AC      ..
MT: GLO CR      .. CHECK COUNTER
    BNZ MPL     .. IF COUNTER IS NOT 0, GO BACK FOR MORE
MPX: GHI AC     .. FINISHED
    SHL         .. CHECK FOR PRODUCT > 15 BITS
    GLO MQ      ..
    LSNF        .. THAT'S HIGH 17 BITS
    XRI 0xFF    .. ALL 00 OR FF
    BNZ _02     ..
    GHI MQ      ..
    LSNF        ..
    XRI 0xFF    ..
_02: ADI 0xFF   .. SET DF IF PRODUCT > 15 BITS
    SEP RETN    .. RETURN
    .. 32/16 BIT SIGNED DIVIDE (2'S COMPLEMENT)
    .. AC=MQ,AC/OPRN
    .. QUOTIENT IN AC, REMAINDER IN MQ
    .. ************ (TO CALL, WRITE) ************
    .. CALL DIVOP; ,A(OPRN)
    ..
DIVOP: LDA LINK .. FETCH OPERAND ADDRESS
    PHI MA      .. TO REGISTER MA
    LDA LINK    ..
    PLO MA      .. FALL INTO DIV0,DIV/DIVQ
    .. 32/16 BIT SIGNED DIVIDE (2'S COMPLEMENT)
    .. OPTION #1: CLEAR MQ AND CHECK FOR ZERO D
    .. **** AC=MQ,AC/M(R(MA))
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL DIV0;
DIV0: SEX MA    .. SET X TO POINT TO DIVISOR (0)
    GHI AC      .. LOOK AT AC SIGN (0)
    SHL         .. COPY IT TO DF (0)
    LDI 0x00    .. EXTEND TO #00 IF POSITIVE (0)
    LSNF        .. (0)
    LDI 0xFF    .. #FF IF NEGATIVE (0)
    PHI MQ      .. GIVING +0 OR -0 IN MQ (0)
    PLO MQ      .. (0)
    LDA MA      .. CHECK FOR ZERO DIVISOR (0)
    OR          .. (0)
    DEC MA      .. (DON'T FORGET TO FIX POINTER) (0)
    SDI 0x00    .. IF ZERO, CALL IT DIVIDE CHECK (0)
    BNF DIV     .. GO ON IF NO DIVIDE CHECK ERR (0)
    SEP RETN    .. AND RETURN WITH DF=1 (0)
    .. OPTION #2: PERMIT 32-BIT DIVIDEND;
    ..
    .. MAKE SURE QUOTIENT DOES NOT EXCEED 16 BITS 
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL DIV;
DIV: GHI MQ     .. SAVE PARTIAL DIVIDEND (/)
    PHI CR      .. INTO CR.1 (/)
    GLO MQ      .. (/)
    PLO CR      .. AND CR.0 (/)
    SHL         .. (/)
    PLO MQ      .. (/)
    GHI MQ      .. DO THE SAME FOR HIGH BYTE (/)
    SHLC        .. REMEMBER CARRY (/)
    PHI MQ      .. (/)
    GHI AC      .. ALSO SHIFT IN AC HIGH (/)
    SHL         .. (/)
    BDF _03     .. (/)
    GLO CR      .. SEE IF MQ.0=0 (/)
    BNZ D2      .. IF NOT GO THROUGH CHECKING STEPS (/)
    GHI CR      .. SEE IF MQ.1 IS #40 (/)
    XRI 0x40    .. WHICH SHOULD RESULT 0 IF TRUE (/)
    BZ  D4      .. IF TRUE, SKIP NORMAL CHECKING
    SKP         ..
_03: INC MQ     .. IF 1, SHIFT INTO MQ (/)
D2: GHI CR      .. CHECK IF HIGH 2 BITS OF MQ ARE THE SAME (/)
    ANI 0xC0    .. TAKE OUT 2 HIGH BITS (/)
    SDI 0x00    .. SEE IF THEY ARE THE SAME (/)
    SHL         .. SHIFT OUT COMPARISON (/)
    LBDF DVXX   .. SET DF AND RETURN (/)
D4: GHI CR      .. LOOK AT THE SIGNS OF DIVND AND DIVSR (/)
    SEX MA      .. POINT X TO DIVISOR (/)
    XOR         .. (/)
    SHL         .. SET DF IF THE SAME (/)
    INC MA      .. MQ.0-(MA) (/)
    GLO MQ      .. MQ.0 TO D (/)
    BDF DVA     .. IF FLAG, MQ.0+(MA) (/)
    SM          .. MQ.0-(MA.0) (/)
    PLO MQ      .. AND STORE BACK INTO MQ.0 (/)
    DEC MA      .. DO THE SAME FOR HIGH BYTE (/)
    GHI MQ      .. LOAD MQ.1 INTO D (/)
    SMB         .. MQ.1-(MA.1) WITH CARRY (/)
    BR  DVB     .. SKIP OVER ADD STEP (/)
DVA: ADD        .. MQ+(MA) (/)
    PLO MQ      .. STORE BACK TO MQ.0 (/)
    DEC MA      .. SAME FOR HIGH BYTE (/)
    GHI MQ      .. MQ.1+(MA.1) (/)
    ADC         .. ADD MQ.1 AND (MA.1) WITH CARRY (/)
DVB: PHI MQ     .. AND STORE BACK INTO MQ.1 (/)
    GHI CR      .. LOOK AT THE SIGN OF DIVND (/)
    SHL         .. SET DF IF DIVND IS NEGATIVE (/)
    LBNF DV2    .. IF NOT, DON'T COMPLEMENT DIFF (/)
    GHI MQ      .. COMPLEMENT MQ (/)
    XRI 0xFF    .. BY XOR RO #FF (/)
    PHI MQ      .. (/)
    GLO MQ      .. (DO THE SAME FOR LOW BYTE) (/)
    XRI 0xFF    .. (/)
    PLO MQ      .. (/)
    INC MQ      .. REMEMBER TO ADD 1 (/)
DV2: GHI MQ     .. NOW LOOK AT THE DIFF OF MQ&(MA) (/)
    LBZ D10     .. IF 0, CHECK MQ=0 (/)
    SHL         .. CHECK IF MQ IS NEGATIVE (/)
    BDF DDQ     .. IF YES, NO PROBLEM (/)
    BR  D9      .. IF NOT 0 NOR NEG, DIVND IS TOO LARGE (/)
D10: GLO MQ     .. HERE WE CHECK DIFF=0 CASES (/)
    ADI 0xFE    .. IF MQ.0IS NOT EITHER 0 OR 1 (/)
    BDF DVXX    .. THEN DIVND IS NEGATIVE (/)
    XRI 0xFF    .. RESULT 0 IF MQ.0 WAS 1 (/)
    BNZ DVH     .. IF NOT, MUST BE 0, GO TO DVH (/)
    GHI CR      .. SEE IF DIVND IS NEGATIVE (/)
    ANI 0x80    .. RESULT 80 IF YES (/)
    BZ  D9      .. IF DIVND POSITIVE, IT CANNOT DIVIDE (/)
    ADD         .. SEE IF DIVISOR IS POSITIVE
    BDF DVXX    .. IF NEGATIVE, DIVND CANNOT DIVIDE (/)
_05: INC MA     .. AC+(MA) TO MQ (/)
    GLO AC      .. DO ac LOW FIRST (/)
    ADD         .. (/)
    PLO MQ      .. STORE BACK TO LOW MQ (/)
    DEC MA      .. DO THE SAME FOR HIGH BYTE (/)
    GHI AC      .. (/)
    ADC         .. ADD WITH CARRY (/)
    PHI MQ      .. (/)
    INC MA      .. LEAVE MA POINTING TO LOW DIVISOR (/)
    LDX         .. NOW CHECK LOW BIT (MA) IS 0 OR 1 (/)
    SHR         .. SHIFT THAT LOW BIT OUT (/)
    DEC MA      .. REMEMBER TO RESET MA (/)
    GHI MQ      .. READY TO ADD #80 (/)
    LSNF        .. IF LOW BIT OF AC IS 1 (/)
    ADI 0x80    .. TO MQ.1 (/)
    PHI MQ      .. STORE STATUS (/)
    SHL         .. SEE IF MQ > 0 (/)
    GHI CR      .. SEE IF DIVND POSITIVE (/)
    ANI 0x80    .. BY TAKE OUT SIGN (/)
    BNZ _04     .. (/)
    BNF D9      .. EXIT WITH DF=1 (/)
    BR  DDQ     .. OK (/)
_04: BDF DVXX   .. IF NEGATIVE, THEN OUT (/)
    GHI MQ      .. SEE IF MQ=0 (/)
    BNZ DDQ     .. IF NOT, THEN NO PROBLEM (/)
    GLO MQ      .. MAKE SURE MQ.0 IS 0 TOO (/)
    BZ  D9      .. IF YES, THEN RETURN WITH DF=1 (/)
    BR  DDQ     .. OR ELSE GO TO DIVIDE (/)
DVH: GHI CR     .. SEE IF THE OPEARANDS SIGNS DIFF (/)
    XOR         .. BY COMPARING SIGN BITS (/)
    SHL         .. AND TAKE OUT THAT BIT (/)
    BDF _05     .. IF SIGNS DIFF, IT'S OK (/)
    GHI CR      .. OTHERWISE TEST SIGN OF DIVND (/)
    SHL         .. IF POSITIVE, RETURN WITH DF=1 (/)
    BNF D9      .. RETURN WITH DF=1 (/)
    INC MA      .. SEE IF LOW BIT OF AC IS 0 OR 1 (/)
    LDX         .. LOAD THAT IN D (/)
    DEC MA      .. REMEMBER TO RESET MA (/)
    SHR         .. SHIFT THAT BIT OUT (/)
    GLO AC      .. IF AC IS NOT 0, NO PROBLEM (/)
    BNZ DDQ     .. GO TO DIVIDE (/)
    GHI AC      .. READY TO CHECK #80 IF LOW BIT AC IS 1(/)
    BDF _06     .. (/)
    BZ  D9      .. NO GOOD, RETURN WITH DF=1 (/)
    BR  DDQ     .. ANY THING ELSE IS OK (/)
_06: XRI 0x80   .. IF AC.1 IS'NT #80 (/)
    BNZ DDQ     .. IT'S OK (/)
D9: SMI 0x00    .. DF IS SET TO 1 (/)
DVXX: GHI CR    .. PUT ORIGINAL DIVND (/)
    PHI MQ      .. INTO MQ (/)
    GLO CR      .. (/)
    PLO MQ      .. (/)
    SEP RETN    .. AND RETURN WITH DF=1 (/)
DDQ: GHI CR     .. PUT ORIGINAL DIVND (/)
    PHI MQ      .. BACK INTO MQ (/)
    GLO CR      .. (/)
    PLO MQ      ..(/)
    .. OPTION #3: ASSUME BENIGN PROGRAM: NO CHECK
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL DIVQ;
DIVQ: GHI MQ    .. LOOK AT DVIDEND SIGN
    SHL         .. IF POSITIVE,
    LDI 0x90    .. PLAN TO BEGIN WITH SUBTRACT,
    LSNF        .. (ALSO SAVE SIGN OF DIVIDEND)
    LDI 0x50    .. OTHERWISE BEGIN WITH ADD
    PLO CR      .. SET ITERATION COUNT IN CR.0
DVL: GLO MQ     .. SHIFT MQ LEFT 1 BIT
    SHL         .. SHIFT LEFT MQ.0
    PLO MQ      ..
    GHI MQ      .. SAME FOR HIGHT BYTE
    SHLC        .. SHIFT LEFT WITH CARRY
    PHI MQ      ..
    GLO AC      .. SHIFT AC LEFT 1 BIT
    SHL         .. SHIFT AC.0
    PLO AC      ..
    GHI AC      ..
    SHLC        .. SHIFT WITH CARRY
    PHI AC      ..
    BNF _07     .. BIT SHIFTED OUT OF AC.1
    INC MQ      .. GOES INTO MQ.0
_07: SEX MA     ..
    GLO CR      .. NOW, WAS THAT ADD, OR SUBTRACT ?
    XOR         .. IT DEPENDS ON SAVED FLAG,
    SHL         .. AND SIGN OF DIVISOR
    INC MA      .. MQ.0-(MA.0)
    GLO MQ      ..
    BNF DSA     .. IF NO FLAG, MQ.0+(MA.0)
    SM          ..
    PLO MQ      ..
    DEC MA      .. FIX X PTR
    GHI MQ      .. DO THE SAME FOR HIGH BYTE
    SMB         .. REMEMBER THAT BORROW BIT
    BR  DSM     .. SKIP OVER ADD STEPS
DSA: ADD        .. MQ+(MA)
    PLO MQ      .. 
    DEC MA      .. DO THE SAME FOR HIGH BYTE
    GHI MQ      ..
    ADC         .. ADD WITH CARRY
DSM: PHI MQ     .. STORE BACK TO MQ.1
    DEC CR      .. COUNT DOWN ITERATION COUNTER
    GLO CR      ..
    ANI 0x7F    ..
    BNF _08     .. TEST CARRY OUT OF ADD/SUBTRACT
    INC AC      .. IF 1, SHIFT INTO QUOTIENT,
    ORI 0x80    .. AND FLAG NEXT OP AS SUBTRACT
_08: PLO CR     .. OTHERWISE IT'S ADD
    ANI 0x3F    .. LOOK AT COUNTER:
    BNZ DVL     .. IF NOT 0, LOOP BACK
    BDF DVR     .. AT END, CHECK REMAINDER ADJUST
    SKP         ..
DVC: INC AC     .. (FINAL DIVIDE STEP)
    XOR         .. BE SURE TO GET POLARITY
    SHL         .. OF ADJUSTMENT RIGHT
    INC MA      .. YES, ADD DIVISOR BACK ON
    GLO MQ      .. TO CORRECT FOR FINAL SUBTRACT
    BDF DVM     .. (ADDING NEGATIVE IS SUBT.)
    ADD         .. WHICH SHOULDN'T HAVE
    PLO MQ      ..
    DEC MA      ..
    GHI MQ      ..
    ADC         .. ADD WITH CARRY
    BR  _09     ..
DVM: SM         .. SAME THING
    PLO MQ      .. EXCEPT FOR NEGATIVE DIVISOR
    DEC MA      ..
    GHI MQ      ..
    SMB         ..
_09: PHI MQ     ..
DVR: GHI MQ     .. IF REMAINDER IS NOT ZERO,
    BNZ _10     ..
    GLO MQ      ..
    BZ  DVN     .. BUT IT IS, NO PROBLEM
_10: GLO CR     .. IF NOT ZERO
    SHL         .. IT SHOULD BE SAME SIGN
    SHL         .. AS ORIGINAL DIVIDEND
    GHI MQ      ..
    LSDF        ..
    XRI 0x80    ..
    ADI 0x80    .. IF NOT, WE NEED
    BNF DVC     .. ONE MORE DIVIDE ITERATION.
DVN: LDX        .. FINALLY, IF DIVISOR NEGATIVE,
    SHL         ..
    BNF DVX     .. (IT' NOT: WE ARE DONE)
    GLO AC      .. COMPLEMENT QUOTIENT
    XRI 0xFF    .. BY INVERTING IT
    PLO AC      ..
    GHI AC      ..
    XRI 0xFF    ..
    PHI AC      ..
    INC AC      .. THEN INCREMENTING
    ADI 0x00    .. ALSO CLEAR DF
DVX: SEP RETN   .. DF=0 IF DIVIDE SUCCESSFULL
    ..
    .. 16-BIT ADD TO AC, OPERAND ADDRESS IN CALL
    .. **** AC=AC+OPRN OPRN=2 BYTE OPERAND
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL ADDOP; ,A(OPRN)
    ..
    ..
ADDOP: LDA LINK .. FETCH OPERAND ADDRESS
    PHI MA      .. TO REGISTER MA
    LDA LINK    .. 
    PLO MA      .. FALL INTO ADD
    .. 16-BIT ADD TO AC, OPERAND ADDRESS IN REG
    .. CALL HERE IF OPRN ADDRESS
    .. IS IN REGISTER MA
    .. **** AC=AC+M(R(MA))
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL ADD;
    ..
ADD: SEX MA     .. CHECK SIGN BIT OF AC
    GHI AC      .. GET THE OPERAND
    XOR         .. AND OPERAND @MA
    XRI 0x80    .. RESULT 1 IF DIFF
    STR SP      .. STORE ON STACK
    INC MA      .. POINT TO LOW 8 BITS
    GLO AC      .. FETCH AC LOW 8
    ADD         .. ADD LOW 8 FROM MEMORY
    PLO AC      .. PUT IT BACK
    DEC MA      .. POINT TO HIGH 8 MEMORY LOCATION
    GHI AC      ..
    ADC         .. ADD HIGH BYTE WITH CARRY
    PHI AC      .. PUT IN AC
    LDN SP      .. LOAD THE STORED COMPARING SIGN BIT
    SHL         .. RESET STACK PTR
    BNF _11     .. NOT POSSIBLE
    GHI AC      .. OTHERWISE SEE IF SUM IS RIGHT
    XOR         .. BY COMPARING SIGN BITS
ADDRT: SHL      .. SHIFT OUT 0 INTO DF
_11: SEP RETN   .. RETURN TO MAIN
    ..
    .. 16-BIT CONSTANT ADD TO AC
    .. CALL HERE FOR ADD CONTANT TO AC
    .. **** AC=AC+CONSTANT
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL ADDCON; CONSTANT
    ..
ADDCON: GLO LINK .. COPY LINK TO MA
    PLO MA      ..
    GHI LINK    ..
    PHI MA      ..
    INC LINK    .. INCREMENT PAST DATUM
    INC LINK    ..
    BR  ADD     .. GO ADD
    ..
    .. 16-BIT ADD FROM TOP OF STACK
ADDST: GHI SP   .. COPY STACK POINTER
    PHI MA      .. TO MA REGISTER
    GLO SP      ..
    PLO MA      ..
    INC MA      .. ADVANCE TO SUB-TOP (@)
    INC MA      .. (@)
    INC MA      .. (@)
    BR  ADD     .. GO DO IT
    ..
    ..
    ..
    .. PUSH AC INTO STACK
    .. STACK POINTER = SP
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL PUSHAC;
    .. PUSH AC (UNDER TOP OF STACK)
PUSHAC: GHI SP  .. COPY STACK POINTER TO MA
    PHI MA      ..
    GLO SP      ..
    PLO MA      ..
    INC MA      .. NOW SLICE OFF TOP 2 BYTES,
    INC MA      ..  
    SEX SP      ..
    LDN MA      .. TO MAKE A 2-BYTE HOLE.
    STXD        ..
    DEC MA      ..
    LDN MA      ..
    STXD        ..
    GHI AC      .. NOW STUFF AC INTO THE HOLE
    STR MA      ..
    GLO AC      ..
    INC MA      ..
    STR MA      ..
    DEC MA      .. LEAVE MA POINTING TO IT
    SEP RETN    .. (AC UNCHANGED)
    .. PUSH CR, MA, MQ (UNDER TOP OF STACK)
PUSHCQ: GLO MA  .. FISRT PUSH MA ONTO TOP
    SEX SP      ..
    STXD        ..
    GHI MA      ..
    STR SP      ..
    GHI SP      .. NOW COPY SP TO MA
    PHI MA      ..
    GLO SP      ..
    PLO MA      ..
    DEC SP      ..
    INC MA      .. THEN ADJUST IT
    INC MA      ..
    INC MA      .. TO POINT INTO OLD TOP
    GLO MQ      .. CONTINUE PUSHING MQ
    STXD        ..
    GHI MQ      ..
    STXD        ..
    LDN MA      ..
    STXD        ..
    DEC MA      ..
    LDN MA      ..
    STXD        ..
    GHI CR      .. FINALLY INSERT CR IN HOLE
    STR MA      ..
    INC MA      ..
    GLO CR      ..
    STR MA      ..
    SEP RETN    .. (MA IS GARBAGE OUT)
    ..
    .. POP STACK INTO MQ, MA, CR (UNDER TOP OF STACK)
POPCQ: INC SP   ..
    GHI SP      .. COPY STACK POINTER TO MA
    PHI MA      ..
    GLO SP      ..
    PLO MA      ..
    INC MA      .. ADJUST POINTER TO CR DATUM
    INC MA      ..
    INC MA      ..
    INC MA      ..
    INC MA      ..
    INC MA      ..
    LDA MA      .. FETCH IT
    PHI CR      ..
    LDN MA      ..
    PLO CR      ..
    DEC MA      .. COPY TOP OF STACK INTO GAP
    LDA SP      ..
    STR MA      ..
    INC MA      ..
    LDA SP      ..
    STR MA      ..
    LDA SP      .. THEN POP MQ
    PHI MQ      ..
    LDA SP      ..
    PLO MQ      ..
    LDA SP      .. FINALLY POP MA
    PHI MA      ..
    LDN SP      ..
    PLO MA      ..
    SEP RETN    ..
    .. 16-BIT ACCUMULATOR LOAD (ADDRESS IN CALL)
    .. **** AC=OPRN
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL LOADOP; ,A(OPRN)
    ..
LOADOP: LDA LINK .. FETCH ADDRESS
    PHI MA      .. TO MA REGISTER
    LDA LINK    ..
    PLO MA      .. FALL INTO LOAD
    .. 16-BIT ACCUMULATOR LOAD (ADDRESS IN MA)
    .. CALL HERE IF OPERAND
    .. ADDRESS IN REGISTER MA
    .. **** AC=M(R(MA))
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL LOAD;
    ..
LOAD: LDA MA    .. FETCH HIGH 8
    PHI AC      ..
    LDA MA      .. NOW LOW 8
    PLO AC      .. LEAVE MA AT NEXT DOUBLE-BYTE
    SEP RETN    .. GEE, THAT WAS QUICK
    ..
    ..
    .. 16-BIT ACCUMULATOR LOAD FROM CONSTANT
    .. **** AC=CONSTANT
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL LOADCON; CONSTANT
    ..
LOADCON: LDA LINK .. FETCH HIGH 8
    PHI AC      ..
    LDA LINK    .. THEN LOW 8
    PLO AC      ..
    SEP RETN    ..
    ..
    .. 16-BIT ACCUMULATOR STORE (ADDRESS IN CALL)
    .. **** OPRN=AC
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL STOROP; ,A(OPRN)
    ..
STOROP: LDA LINK .. FETCH ADDRESS IN MA
    PHI MA      ..
    LDA LINK    ..
    PLO MA      .. THAN FALL INTO STORE
    .. 16-BIT ACCUMULATOR STORE (ADDRESS IN MA)
    .. **** M(R(MA))=AC
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL STORE;
    ..
STORE: GHI AC   .. FISRT HIGH 8
    STR MA      ..
    INC MA      .. INCREMENT MA, SINCE STR DOESN'T
    GLO AC      .. NOW LOW 8
    STR MA      ..
    INC MA      .. LEAVE POINTING TO NEXT WORD
    SEP RETN    .. QUIT
    ..
    ..
    .. 16-BIT COMPARE, OPERAND ADDRESS IN CALL
    .. **** AC-OPRN (DF SET IF 0 OR +)
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL COMPOP; ,A(OPRN)
    ..
COMPOP: LDA LINK .. FETCH ADDRESS
    PHI MA      .. TO MA REGISTER
    LDA LINK    ..
    PLO MA      ..
    .. 16-BIT COMPARE, OPERAND ADDRESS IN REGISTER
    .. CALL HERE IF OPERAND
    .. ADDRESS IN REGISTER MA
    .. **** AC-M(R(MA)) (DF SET IF 0 OR +)
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL COMP;
    ..
COMP: SEX MA    .. COMPARE HIGH 8 FIRST
    GHI AC      .. CHECK IF SIGN OF OPERANDS ARE SAME
    XOR         .. BY LOOKING AT THE HIGHETS SIGN BIT
    ANI 0x80    .. RESULT '1' IF NEGATIVE
    BNZ CNE     .. IF NOT, GO TO CNE
    GHI AC      ..
    SM          ..
    BNZ CMPX    .. NOT EQUAL QUIT
    INC MA      .. TRY LOW 8
    GLO AC      ..
    SM          ..
    DEC MA      .. LEAVE MA POINTING TO IT
    SKP         ..
CNE: ADD        .. SEE IF OPERAND IS NEGATIVE
CMPX: SEP RETN  .. DF=1 IF AC>=MA
    ..
    ..
    ..
    ..
    .. TEST 16-BIT ACCUMULATOR SIGN/ZERO
_12: INC LINK   .. SKIP OVER NON ZERO RETURN
    INC LINK    ..
_13: SEP RETN   ..
TEST: GHI AC    .. FIRST LOOK AT SIGN
    SHL         .. SET DF IF MINUS
    GHI AC      .. NOW CHECK FOR 0
    BNZ _12     .. NO
    GLO AC      ..
    BZ  _13     .. GO TAKE ZERO EXIT
    BR  _12     .. GO TAKE NON-ZERO EXIT
    .. POP STACK (INTO AC) (UNDER TOP)
    .. POPS 2 BYTES FROM STACK IN AC
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL POPAC;
POPAC: SMI 0x00 .. SET DF TO REMEMBER THIS ENTRY
    LSKP        ..
POP: ADI 0x00   .. CLEAR DF FOR THIS ENTRY
    INC SP      ..
    GHI SP      .. COPY SP TO MA
    PHI MA      ..
    GLO SP      ..
    PLO MA      ..
    INC MA      .. ADJUST SUB-TOP OF STACK
    INC MA      ..
    BNF _14     ..
    LDA MA      .. POPPING AC GET DATUM
    PHI AC      ..
    LDN MA      ..
    PLO AC      ..
    DEC MA      ..
_14: LDA SP     .. NOW CLOSE UP THE GAP
    STR MA      ..
    INC MA      ..
    LDN SP      ..
    STR MA      ..
    INC MA      ..
    SEP RETN    .. MA POINTS TO SUB-TOP
    ..
    .. DECIMAL TO BINARY CONVERSION
    .. **** AC=DECIMAL NUMBER OF N BYTES
    .. DECIMAL NUMBER = SIGN,NN,.....,N1,N0
    .. SIGN=#0B +
    .. SIGN=#0D -
    .. N0=10**0 DIGIT
    .. N1=10**1 DIGIT
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL CDB; ,A(NUMBER); LENGTH
CDB: LDA LINK   .. GET NUM ADDRESS
    PHI AR      .. AND STORE IN RCA
    LDA LINK    ..
    PLO AR      ..
    LDA LINK    .. GET LENGTH
    SMI 0x01    .. MINUS SIGN BYTE
    PLO NR      .. AND STORE IN RB
    LDI 0x00    .. CLEAR RF
    PLO AC      ..
    PHI AC      ..
    LDN AR      .. CHECK SIGN BYTE
    XRI 0x0D    .. MINUS ?
    PHI NR      .. INTO NR.1
LOOP: INC AR    .. GRAB THE FIRST DIGIT
    SEX SP      .. FIX X PTR
    LDN AR      .. ALWAR HIGH BYTE (FOR ASCII)
    ANI 0x0F    .. 
    STR SP      .. PUT IT BACK
    GLO AC      .. ADD THAT DIGIT TO CCUM
    ADD         ..
    PLO AC      ..
    GHI AC      ..
    ADCI 0x00   .. REMEMBER CARRY OVER
    PHI AC      ..
    BDF OVFLW   .. EXECEEDS ACCUM LIMIT ?
    DEC NR      .. DEC DIGIT COUNTER
    GLO NR      .. SEE IF IT IS 0 ?
    BZ  FINAL   .. YES, THEN DONE
    GLO AC      .. OTHERWISE MULTIPLY THE ACC BY 10
    SHL         .. 
    STXD        ..
    GHI AC      ..
    SHLC        .. CARRY OVER
    STXD        ..
    BDF _15     .. EXCEEDED ACC LIMIT
    LDI 0x02    ..
MPY3: STR SP    .. LOOP COUNT
    GLO AC      ..
    SHL         .. NOW SHIFT AC OVER 4 TIMES MORE
    PLO AC      ..
    GHI AC      .. SAME FOR AC.1
    SHLC        ..
    PHI AC      ..
    BDF _15     .. IF OVERFLOW, RESET STACK
    LDN SP      .. CHECK LOOP COUNT
    BZ  MPY10   .. AFTER MULTIPLY BY 8
    SMI 0x01    .. OR ELSE DEC LOOP COUNT
    BR  MPY3    .. BACK FOR MORE ADDITION
MPY10: INC SP   .. RECOVER LOOP COUNT
    GHI AC      .. ADD HIGH BYTE
    ADD         .. ADD 8ACC TO 2ACC
    PHI AC      ..
    BDF _16     .. RESULT DF IF OVFLOW
    INC SP      .. RESET STACK PNTR
    GLO AC      .. SAME FOR AC.0
    ADD         ..
    PLO AC      ..
    GHI AC      .. FOR CARRY OUT
    ADCI 0x00   ..
    PHI AC      ..
    BNF LOOP    .. IF NOT OVFLW, GO BACK FOR MORE
    LSKP        .. SKIP STACK RESET
_15: INC SP     .. RESET STACK PTR
_16: INC SP     ..
OVFLW: SEP RETN .. DF=1
FINAL: GHI AC   .. CHECK IF EXCEED MAX POS NUM LIMT
    ADI 0x80    ..
    BNZ CP      .. IF NOT POSSIBLE, SKIP
    GLO AC      ..
    BNZ CP      .. IF NOT GO TO COMP
    GHI NR      .. SEE IF IT IS POSITIVE
    ADI 0xFF    .. SET DF ACCORDINGLY
_17: SEP RETN   ..
CP: BDF _17     .. OVERFLOWED !
    GHI NR      .. TEST FOR SIGN
    BNZ EXIT    .. IF POS, DONE
    GLO AC      .. IF NEG, SUBTRACT FROM 0
    SDI 0x00    ..
    PLO AC      ..
    GHI AC      ..
                ..
    SDBI 0x00   ..
    PHI AC      ..
    ..
    .. BINARY TO DECIMAL CONVERSION
    .. **** DECIMAL NUMBER=AC
    .. DECIMAL NUMBER = SIGN,NN,.....,N1,N0
    .. SIGN=#0B +
    .. SIGN=#0D -
    .. N0=10**0 DIGIT
    .. N1=10**1 DIGIT, ETC
    .. ************ (TO CALL, WRITE) ************
    .. **** CALL CBD; ,A(NUMBER); LENGTH
EXIT: SEP RETN  ..
CBD: LDA LINK   .. GET THE ADDRESS
    PHI AR      .. AND STORE IN RA
    LDA LINK    .. SAME FOR LOW BYTE
    PLO AR      ..
    LDA LINK    .. GET LENGTH
    SMI 0x01    .. SUBTRACT FOR SIGN BYTE
    PLO NR      .. STORE IN NR.0
    PHI NR      .. AND NR.1
    LDI 0x0F    .. NUM OF ITERATIONS
    PLO MA      .. STORE IN MA.0
    GHI AC      .. TEST FOR SIGN
    SHL         ..
    LDI 0x0B    .. IF DF=0, IT IS POS
    BNF POS     ..
    GLO AC      .. OTHERWISE CONVERT IT TO POS
    SDI 0x00    ..
    PLO AC      ..
    GHI AC      ..
    SDBI 0x00   ..
    PHI AC      ..
    LDI 0x0D    .. MINUS SIGN
    LSKP        ..
_19: LDI 0x00    ..
POS: STR AR     .. UT IT IN SIGN BYTE
    GLO NR      .. CHECK DIGIT COUNTER
    BZ  _18     .. GO BACK FOR MORE ITERATION
    INC AR      .. GO TO NEXT DIGIT
    DEC NR      .. DEC DIGIT CNTR
    BR  _19     .. GO BACK FOR MORE CLEAR
    SEX SP      ..
_18: GHI NR      .. RESET DIGIT CNTR
    PLO NR      ..
LOOP1: GLO AC   .. SHIFT BIT OF AC OUT
    SHL         ..
    PLO AC      ..
    GHI AC      ..
    SHLC        .. SAME FOR AC.1
    PHI AC      ..
    LDN AR      ..
    ADCI 0x00   .. ADD TO LOWEST DIGIT
    STR AR      ..
    GLO MA      .. FOR MORE ITERATION ?
    BNZ NEXT     .. CONTINUE IF MORE ITERATION
END: SEP RETN   ..
NEXT: LDN AR    .. LOAD DIGIT
    SHLC        .. SHIFT LEFT OVER ONCE
    STR AR      .. PUT IT BACK
    SMI 0x0A    .. NEED TO INC NEXT DIGIT ?
    BNF _20     .. SKIP IF NOT>10
    STR AR      .. ELSE UPDATE DIGIT
_20: DEC AR      .. GO TO NEXT DIGIT
    DEC NR      .. DEC DIGIT COUNT
    GLO NR      .. CHECK IF 0 ?
    BNZ EXIT    .. IF NOT, DO THE SAME FOR NEXT DIGIT
    BDF END     .. OVERFLOWED
    DEC MA      .. DEC NO OF SHIFTS
    GHI NR      .. RESET ADDRESS PTR
    STR SP      .. PUT DIGIT ON STACK
    GLO AR      ..
    ADD         ..
    PLO AR      ..
    GHI AR      ..
    ADCI 0x00   ..
    PHI AR      ..
    BR _18      ..
                .. THE END