
# REGISTERS ALIAS

| Name | Number | Description | 
| --- | --- | --- |  
| __SP__ | R2 | Stack pointer |
| __PC__ | R3 | Program counter |
| __CALL__ | R4 | Pointer to CALL routine |
| __RETN__ | R5 | Pointer to RETURN routine |
| __LINK__ | R6 | Pointer to arguments list |
| __AR__ | RA | Pointer to BCD digits in memory |
| __NR__ | RB | Counter for the number of BCD digits |
| __CR__ | RC | Scratchpad and counter |
| __MA__ | RD | Operand pointer |
| __MQ__ | RE | Accumulator sign extension |
| __AC__ | RF | Accumulator |

# INSTRUCTIONS

| IDL    ||
| ------ | --- |
| Desc   | Idle |
| Logic  | R(P) <- R(P)-1; Bus <- M(R(0)) until an interrupt or DMA request |
| Cycles | 2 |
| Opcode | 0x00 |
| Param  | NONE |


| LDN    ||
| ------ | --- |
| Desc   | Load D via N (for N = 1 to F) |
| Logic  | D <- M(R(N)) for N != 0 |
| Cycles | 2 |
| Opcode | 0x0N |
| Param  | REGISTER |
    

| INC    ||
| ------ | --- |
| Desc   | Increment Register |
| Logic  | R(N) <- R(N)+1 |
| Cycles | 2 |
| Opcode | 0x1N |
| Param  | REGISTER |
    

| DEC    ||
| ------ | --- |
| Desc   | Decrement Register |
| Logic  | R(N) <- R(N)-1 |
| Cycles | 2 |
| Opcode | 0x2N |
| Param  | REGISTER |
    

| BR     ||
| ------ | --- |
| Desc   | Branch unconditionally |
| Logic  | R(P).0 <- M(R(P)) |
| Cycles | 2 |
| Opcode | 0x30 aa |
| Param  | BYTE ADDRESS |
    

| BQ     ||
| ------ | --- |
| Desc   | Branch if Q is on |
| Logic  | if Q==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| Cycles | 2 |
| Opcode | 0x31 aa |
| Param  | BYTE ADDRESS |
    

| BZ     ||
| ------ | --- |
| Desc   | Branch on Zero |
| Logic  | if D==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| Cycles | 2 |
| Opcode | 0x32 aa |
| Param  | BYTE ADDRESS |
    

| BDF    ||
| ------ | --- |
| Desc   | Branch if DF is 1 |
| Logic  | if DF==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| Cycles | 2 |
| Opcode | 0x33 aa |
| Param  | BYTE ADDRESS |
    

| B1     ||
| ------ | --- |
| Desc   | Branch on External Flag 1 |
| Logic  | if EF1==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| Cycles | 2 |
| Opcode | 0x34 aa |
| Param  | BYTE ADDRESS |
    

| B2     ||
| ------ | --- |
| Desc   | Branch on External Flag 2 |
| Logic  | if EF2==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| Cycles | 2 |
| Opcode | 0x35 aa |
| Param  | BYTE ADDRESS |
    

| B3     ||
| ------ | --- |
| Desc   | Branch on External Flag 3 |
| Logic  | if EF3==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| Cycles | 2 |
| Opcode | 0x36 aa |
| Param  | BYTE ADDRESS |
    

| B4     ||
| ------ | --- |
| Desc   | Branch on External Flag 4 |
| Logic  | if EF4==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| Cycles | 2 |
| Opcode | 0x37 aa |
| Param  | BYTE ADDRESS |
    

| SKP    ||
| ------ | --- |
| Desc   | Skip one byte |
| Logic  | R(P) <- R(P)+1 |
| Cycles | 2 |
| Opcode | 0x38 |
| Param  | NONE |
    

| BNQ    ||
| ------ | --- |
| Desc   | Branch if Q is off |
| Logic  | if Q==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| Cycles | 2 |
| Opcode | 0x39 aa |
| Param  | BYTE ADDRESS |
    

| BNZ    ||
| ------ | --- |
| Desc   | Branch on Not Zero |
| Logic  | if D!=0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| Cycles | 2 |
| Opcode | 0x3A aa |
| Param  | BYTE ADDRESS |
    

| BNF    ||
| ------ | --- |
| Desc   | Branch if DF is |
| Logic  | if DF!=0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| Cycles | 2 |
| Opcode | 0x3B aa |
| Param  | BYTE ADDRESS |
    

| BN1    ||
| ------ | --- |
| Desc   | Branch on Not External Flag 1 |
| Logic  | if EF1==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| Cycles | 2 |
| Opcode | 0x3C  aa |
| Param  | BYTE ADDRESS |
    

| BN2    ||
| ------ | --- |
| Desc   | Branch on Not External Flag 2 |
| Logic  | if EF2==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| Cycles | 2 |
| Opcode | 0x3D aa |
| Param  | BYTE ADDRESS |
    

| BN3    ||
| ------ | --- |
| Desc   | Branch on Not External Flag 3 |
| Logic  | if EF3==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| Cycles | 2 |
| Opcode | 0x3E aa |
| Param  | BYTE ADDRESS |
    

| BN4    ||
| ------ | --- |
| Desc   | Branch on Not External Flag 4 |
| Logic  | if EF4==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| Cycles | 2 |
| Opcode | 0x3F aa |
| Param  | BYTE ADDRESS |
    

| LDA    ||
| ------ | --- |
| Desc   | Load D and Advance |
| Logic  | D <- M(R(N)); R(N) <- R(N)+1 |
| Cycles | 2 |
| Opcode | 0x4N |
| Param  | REGISTER |
    

| STR    ||
| ------ | --- |
| Desc   | Store D into memory |
| Logic  | M(R(N)) <- D |
| Cycles | 2 |
| Opcode | 0x5N |
| Param  | REGISTER |
    

| IRX    ||
| ------ | --- |
| Desc   | Increment R(X) |
| Logic  | R(X) <- R(X)+1 |
| Cycles | 2 |
| Opcode | 0x60 |
| Param  | NONE |
    

| OUT    ||
| ------ | --- |
| Desc   | Output from memory (for p = 1 to 7) |
| Logic  | BUS <- M(R(X)) ; R(X) <- R(X)+1 |
| Cycles | 2 |
| Opcode | 0x6P |
| Param  | IO |
    

| INP    ||
| ------ | --- |
| Desc   | Input to memory and D (for p = 9 to F) |
| Logic  | M(R(X)) <- BUS; D <- BUS |
| Cycles | 2 |
| Opcode | 0x6P |
| Param  | IO |
    

| RET    ||
| ------ | --- |
| Desc   | Return |
| Logic  | (XP) <- M(R(X)); R(X) <- R(X)+1; IE <- 1 |
| Cycles | 2 |
| Opcode | 0x70 |
| Param  | NONE |
    

| DIS    ||
| ------ | --- |
| Desc   | Return and Disable Interrupts |
| Logic  | (XP) <- M(R(X)); R(X) <- R(X)+1; IE <- 0 |
| Cycles | 2 |
| Opcode | 0x71 |
| Param  | NONE |
    

| LDXA   ||
| ------ | --- |
| Desc   | Load D via R(X) and Advance |
| Logic  | D <- M(R(X)); R(X) <- R(X)+1 |
| Cycles | 2 |
| Opcode | 0x72 |
| Param  | NONE |
    

| STXD   ||
| ------ | --- |
| Desc   | Store D via R(X) and Decrement |
| Logic  | M(R(X)) <- D; R(X) <- R(X)-1 |
| Cycles | 2 |
| Opcode | 0x73 |
| Param  | NONE |
    

| ADC    ||
| ------ | --- |
| Desc   | Add with Carry |
| Logic  | DF,D <- M(R(X)) + D + DF |
| Cycles | 2 |
| Opcode | 0x74 |
| Param  | NONE |
    

| SDB    ||
| ------ | --- |
| Desc   | Subtract D from memory with Borrow |
| Logic  | DF,D <- M(R(X)) - D - complement(DF) |
| Cycles | 2 |
| Opcode | 0x75 |
| Param  | NONE |
    

| SHRC   ||
| ------ | --- |
| Desc   | Shift D Right with Carry |
| Logic  | shift D right one bit; MSB(D) <- DF ; DF <- LSB(D) |
| Cycles | 2 |
| Opcode | 0x76 |
| Param  | NONE |
    

| SMB    ||
| ------ | --- |
| Desc   | Subtract Memory from D with Borrow |
 | Logic  | DF,D <- D - M(R(X)) - complement(DF) |
| Cycles | 2 |
| Opcode | 0x77 |
| Param  | NONE |
    

| SAV    ||
| ------ | --- |
| Desc   | Save T |
| Logic  | M(R(X)) <- T |
| Cycles | 2 |
| Opcode | 0x78 |
| Param  | NONE |
    

| MARK   ||
| ------ | --- |
| Desc   | Save X and P in T |
| Logic  | T <- XP ; M(R(2)) <- T; X <- P; R(2) <- R(2)-1 |
| Cycles | 2 |
| Opcode | 0x79 |
| Param  | NONE |
    

| REQ    ||
| ------ | --- |
| Desc   | Reset Q |
| Logic  | Q <- 0 |
| Cycles | 2 |
| Opcode | 0x0A |
| Param  | NONE |
    

| SEQ    ||
| ------ | --- |
| Desc   | Set Q |
| Logic  | Q <- 1 |
| Cycles | 2 |
| Opcode | 0x7B |
| Param  | NONE |
    

| ADCI   ||
| ------ | --- |
| Desc   | Add with Carry Immediate |
| Logic  | DF,D <- M(R(P)) + D + DF; R(P) <- R(P)+1 |
| Cycles | 2 |
| Opcode | 0x7C bb |
| Param  | BYTE |
    

| SDBI   ||
| ------ | --- |
| Desc   | Subtract D with Borrow Immediate |
| Logic  | DF,D <- M(R(P)) - D - complement(DF); R(P) <- R(P)+1 |
| Cycles | 2 |
| Opcode | 0x7D bb |
| Param  | BYTE |
    

| SHLC   ||
| ------ | --- |
| Desc   | Shift D Left with Carry |
| Logic  | shift D left one bit; LSB(D) <- DF ; DF <- MSB(D) |
| Cycles | 2 |
| Opcode | 0x7E |
| Param  | NONE |
    

| SMBI   ||
| ------ | --- |
| Desc   | Subtract Memory with Borrow Immediate |
| Logic  | DF,D <- D - M(R(P)) - complement(DF); R(P) <- R(P)+1 |
| Cycles | 2 |
| Opcode | 0x7F bb |
| Param  | BYTE |
    

| GLO    ||
| ------ | --- |
| Desc   | Get Low byte of Register |
| Logic  | D <- R(N).0 |
| Cycles | 2 |
| Opcode | 0x8N | 
| Param  | REGISTER |
    

| GHI    ||
| ------ | --- |
| Desc   | Get High byte of Register |
| Logic  | D <- R(N).1 |
| Cycles | 2 |
| Opcode | 0x9N |
| Param  | REGISTER |
    

| PLO    ||
| ------ | --- |
| Desc   | Put D in Low byte of register |
| Logic  | R(N).0 <- D |
| Cycles | 2 |
| Opcode | 0xAN |
| Param  | REGISTER |
    

| PHI    ||
| ------ | --- |
| Desc   | Put D in High byte of register |
| Logic  | R(N).1 <- D |
| Cycles | 2 |
| Opcode | 0xBN |
| Param  | REGISTER |
    

| LBR    ||
| ------ | --- |
| Desc   | Long Branch unconditionally |
| Logic  | R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1) |
| Cycles | 3 |
| Opcode | 0xC0 aaaa |
| Param  | WORD ADDRESS |
    

| LBQ    ||
| ------ | --- |
| Desc   | Long Branch if Q is on |
| Logic  | if Q==1 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1); else R(P) <- R(P)+2 |
| Cycles | 3 |
| Opcode | 0xC1 aaaa |
| Param  | WORD ADDRESS |
    

| LBZ    ||
| ------ | --- |
| Desc   | Long Branch if Zero |
| Logic  | if D==0 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1); else R(P) <- R(P)+2 |
| Cycles | 3 |
| Opcode | 0xC2 aaaa |
| Param  | WORD ADDRESS |
    

| LBDF   ||
| ------ | --- |
| Desc   | Long Branch if DF is 1 |
| Logic  | if DF==1 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1); else R(P) <- R(P)+2 |
| Cycles | 3 |
| Opcode | 0xC3 aaaa |
| Param  | WORD ADDRESS |
    

| NOP    ||
| ------ | --- |
| Desc   | No Operation |
| Logic  | R(P) <- R(P)+1 |
| Cycles | 3 |
| Opcode | 0xC4 |
| Param  | NONE |
    

| LSNQ   ||
| ------ | --- |
| Desc   | Long Skip if Q is off |
| Logic  | if Q==0 then R(P) <- R(P) + 2 |
| Cycles | 3 |
| Opcode | 0xC5 |
| Param  | NONE |
    

| LSNZ   ||
| ------ | --- |
| Desc   | Long Skip if Not Zero |
| Logic  | if D!=0 then R(P) <- R(P) + 2 |
| Cycles | 3 |
| Opcode | 0xC6 |
| Param  | NONE |
    

| LSNF   ||
| ------ | --- |
| Desc   | Long Skip if DF is 0 |
| Logic  | if DF==0 then R(P) <- R(P) + 2 |
| Cycles | 3 |
| Opcode | 0xC7 |
| Param  | NONE |
    

| LSKP   ||
| ------ | --- |
| Desc   | Long Skip |
| Logic  | R(P) <- R(P) + 2 |
| Cycles | 3 |
| Opcode | 0xC8 |
| Param  | NONE |
    

| LBNQ   ||
| ------ | --- |
| Desc   | Long Branch if Q is off |
| Logic  | if Q==0 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1); else R(P) <- R(P)+2 |
| Cycles | 3 |
| Opcode | 0xC9 aaaa |
| Param  | WORD ADDRESS |
    

| LBNZ   ||
| ------ | --- |
| Desc   | Long Branch if Not Zero |
| Logic  | if D!=0 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1); else R(P) <- R(P)+2 |
| Cycles | 3 |
| Opcode | 0xCA aaaa |
| Param  | WORD ADDRESS |
    

| LBNF   ||
| ------ | --- |
| Desc   | Long Branch if DF is 0 |
| Logic  | if DF==0 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1); else R(P) <- R(P)+2 |
| Cycles | 3 |
| Opcode | 0xCB aaaa |
| Param  | WORD ADDRESS |
    

| LSIE   ||
| ------ | --- |
| Desc   | Long Skip if Interrupts Enabled |
| Logic  | if IE==1 then R(P) <- R(P)+2 |
| Cycles | 3 |
| Opcode | 0xCC |
| Param  | NONE |
    

| LSQ    ||
| ------ | --- |
| Desc   | Long Skip if Q is on |
| Logic  | if Q==1 then R(P) <- R(P)+2 |
| Cycles | 3 |
| Opcode | 0xCD |
| Param  | NONE |
    

| LSZ    ||
| ------ | --- |
| Desc   | Long Skip if Zero |
| Logic  | if D==0 then R(P) <- R(P)+2 |
| Cycles | 3 |
| Opcode | 0xCE |
| Param  | NONE |
    

| LSDF   ||
| ------ | --- |
| Desc   | Long Skip if DF is 1 |
| Logic  | if DF==1 then R(P) <- R(P)+2 |
| Cycles | 3 |
| Opcode | 0xCF |
| Param  | NONE |
    

| SEP    ||
| ------ | --- |
| Desc   | Set P |
| Logic  | P <- N |
| Cycles | 3 |
| Opcode | 0xD0 |
| Param  | REGISTER |
    

| SEX    ||
| ------ | --- |
| Desc   | Set X |
| Logic  | X <- N |
| Cycles | 2 |
| Opcode | 0xEN |
| Param  | REGISTER |
    

| LDX    ||
| ------ | --- |
| Desc   | Load D via R(X) |
| Logic  | D <- M(R(X)) |
| Cycles | 2 |
| Opcode | 0xF0 |
| Param  | NONE |
    

| OR     ||
| ------ | --- |
| Desc   | Logical OR |
| Logic  | D <- M(R(X)) OR D |
| Cycles | 2 |
| Opcode | 0xF1 |
| Param  | NONE |
    

| AND    ||
| ------ | --- |
| Desc   | Logical AND |
| Logic  | D <- M(R(X)) AND D |
| Cycles | 2 |
| Opcode | 0xF2 |
| Param  | NONE |


| XOR    ||
| ------ | --- |
| Desc   | Exclusive OR |
| Logic  | D <- M(R(X)) XOR D |
| Cycles | 2 |
| Opcode | 0xF3 |
| Param  | NONE |
    

| ADD    ||
| ------ | --- |
| Desc   | Add |
| Logic  | DF,D <- M(R(X)) + D |
| Cycles | 2 |
| Opcode | 0xF4 |
| Param  | NONE |
    

| SD     ||
| ------ | --- |
| Desc   | Subtract D from memory |
| Logic  | DF,D <- M(R(X)) - D |
| Cycles | 2 |
| Opcode | 0xF5 |
| Param  | NONE |
    

| SHR    ||
| ------ | --- |
| Desc   | Shift D Right |
| Logic  | Shift D right one bit; MSB(D) <- 0; DF <- LSB(D) |
| Cycles | 2 |
| Opcode | 0xF6 |
| Param  | NONE |
    

| SM     ||
| ------ | --- |
| Desc   | Subtract Memory from D |
| Logic  | DF,D <- D - M(R(X)) |
| Cycles | 2 |
| Opcode | 0xF7 |
| Param  | NONE |
    

| LDI    ||
| ------ | --- |
| Desc   | Load D Immediate |
| Logic  | D <-M(R(P)); R(P) <- R(P)+1 |
| Cycles | 2 |
| Opcode | 0xF8 bb |
| Param  | BYTE |
    

| ORI    ||
| ------ | --- |
| Desc   | OR Immediate |
| Logic  | D <- M(R(P)) OR D; R(P) <- R(P)+1 |
| Cycles | 2 |
| Opcode | 0xF9 bb |
| Param  | BYTE |
    

| ANI    ||
| ------ | --- |
| Desc   | AND Immediate |
| Logic  | D <- M(R(P)) AND D; R(P) <- R(P)+1 |
| Cycles | 2 |
| Opcode | 0xFA bb |
| Param  | BYTE |
    

| XRI    ||
| ------ | --- |
| Desc   | Exclusive OR Immediate |
| Logic  | D <- M(R(P)) XOR D; R(P) <- R(P)+1 |
| Cycles | 2 |
| Opcode | 0xFB bb |
| Param  | BYTE |
    

| ADI    ||
| ------ | --- |
| Desc   | Add Immediate |
| Logic  | DF,D <- M(R(P)) + D; R(P) <- R(P)+1 |
| Cycles | 2 |
| Opcode | 0xFC bb |
| Param  | BYTE |
    

| SDI    ||
| ------ | --- |
| Desc   | Subtract D from memory Immediate |
| Logic  | DF,D <- M(R(P)) - D; R(P) <- R(P)+1 |
| Cycles | 2 |
| Opcode | 0xFD bb |
| Param  | BYTE |
    

| SHL    ||
| ------ | --- |
| Desc   | Shift D Left |
| Logic  | Shift left one bit; LSB(D) <- 0 ; DF <- MSB(D) |
| Cycles | 2 |
| Opcode | 0xFE |
| Param  | NONE |
    

| SMI    ||
| ------ | --- |
| Desc   | Subtract Memory from D Immediate |
| Logic  | DF,D <- D - M(R(P)); R(P) <- R(P)+1 |
| Cycles | 2 |
| Opcode | 0xFF |
| Param  | BYTE |
    

