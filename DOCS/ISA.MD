
# INSTRUCTIONS

| IDL    ||
| ------ | --- |
| desc   | Idle |
| logic  | R(P) <- R(P)-1; Bus <- M(R(0)) until an interrupt or DMA request |
| cycles | 2 |
| opcode | 0x00 |
| param  | NONE |


| LDN    ||
| ------ | --- |
| desc   | Load D via N (for N = 1 to F) |
| logic  | D <- M(R(N)) for N != 0 |
| cycles | 2 |
| opcode | 0x0N |
| param  | REGISTER |
    

| INC    ||
| ------ | --- |
| desc   | Increment Register |
| logic  | R(N) <- R(N)+1 |
| cycles | 2 |
| opcode | 0x1N |
| param  | REGISTER |
    

| DEC    ||
| ------ | --- |
| desc   | Decrement Register |
| logic  | R(N) <- R(N)-1 |
| cycles | 2 |
| opcode | 0x2N |
| param  | REGISTER |
    

| BR     ||
| ------ | --- |
| desc   | Branch unconditionally |
| logic  | R(P).0 <- M(R(P)) |
| cycles | 2 |
| opcode | 0x30 aa |
| param  | BYTE ADDRESS |
    

| BQ     ||
| ------ | --- |
| desc   | Branch if Q is on |
| logic  | if Q==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| cycles | 2 |
| opcode | 0x31 aa |
| param  | BYTE ADDRESS |
    

| BZ     ||
| ------ | --- |
| desc   | Branch on Zero |
| logic  | if D==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| cycles | 2 |
| opcode | 0x32 aa |
| param  | BYTE ADDRESS |
    

| BDF    ||
| ------ | --- |
| desc   | Branch if DF is 1 |
| logic  | if DF==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| cycles | 2 |
| opcode | 0x33 aa |
| param  | BYTE ADDRESS |
    

| B1     ||
| ------ | --- |
| desc   | Branch on External Flag 1 |
| logic  | if EF1==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| cycles | 2 |
| opcode | 0x34 aa |
| param  | BYTE ADDRESS |
    

| B2     ||
| ------ | --- |
| desc   | Branch on External Flag 2 |
| logic  | if EF2==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| cycles | 2 |
| opcode | 0x35 aa |
| param  | BYTE ADDRESS |
    

| B3     ||
| ------ | --- |
| desc   | Branch on External Flag 3 |
| logic  | if EF3==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| cycles | 2 |
| opcode | 0x36 aa |
| param  | BYTE ADDRESS |
    

| B4     ||
| ------ | --- |
| desc   | Branch on External Flag 4 |
| logic  | if EF4==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| cycles | 2 |
| opcode | 0x37 aa |
| param  | BYTE ADDRESS |
    

| SKP    ||
| ------ | --- |
| desc   | Skip one byte |
| logic  | R(P) <- R(P)+1 |
| cycles | 2 |
| opcode | 0x38 |
| param  | NONE |
    

| BNQ    ||
| ------ | --- |
| desc   | Branch if Q is off |
| logic  | if Q==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| cycles | 2 |
| opcode | 0x39 aa |
| param  | BYTE ADDRESS |
    

| BNZ    ||
| ------ | --- |
| desc   | Branch on Not Zero |
| logic  | if D!=0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| cycles | 2 |
| opcode | 0x3A aa |
| param  | BYTE ADDRESS |
    

| BNF    ||
| ------ | --- |
| desc   | Branch if DF is |
| logic  | if DF!=0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| cycles | 2 |
| opcode | 0x3B aa |
| param  | BYTE ADDRESS |
    

| BN1    ||
| ------ | --- |
| desc   | Branch on Not External Flag 1 |
| logic  | if EF1==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| cycles | 2 |
| opcode | 0x3C  aa |
| param  | BYTE ADDRESS |
    

| BN2    ||
| ------ | --- |
| desc   | Branch on Not External Flag 2 |
| logic  | if EF2==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| cycles | 2 |
| opcode | 0x3D aa |
| param  | BYTE ADDRESS |
    

| BN3    ||
| ------ | --- |
| desc   | Branch on Not External Flag 3 |
| logic  | if EF3==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| cycles | 2 |
| opcode | 0x3E aa |
| param  | BYTE ADDRESS |
    

| BN4    ||
| ------ | --- |
| desc   | Branch on Not External Flag 4 |
| logic  | if EF4==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1 |
| cycles | 2 |
| opcode | 0x3F aa |
| param  | BYTE ADDRESS |
    

| LDA    ||
| ------ | --- |
| desc   | Load D and Advance |
| logic  | D <- M(R(N)); R(N) <- R(N)+1 |
| cycles | 2 |
| opcode | 0x4N |
| param  | REGISTER |
    

| STR    ||
| ------ | --- |
| desc   | Store D into memory |
| logic  | M(R(N)) <- D |
| cycles | 2 |
| opcode | 0x5N |
| param  | REGISTER |
    

| IRX    ||
| ------ | --- |
| desc   | Increment R(X) |
| logic  | R(X) <- R(X)+1 |
| cycles | 2 |
| opcode | 0x60 |
| param  | NONE |
    

| OUT    ||
| ------ | --- |
| desc   | Output from memory (for p = 1 to 7) |
| logic  | BUS <- M(R(X)) ; R(X) <- R(X)+1 |
| cycles | 2 |
| opcode | 0x6P |
| param  | IO |
    

| INP    ||
| ------ | --- |
| desc   | Input to memory and D (for p = 9 to F) |
| logic  | M(R(X)) <- BUS; D <- BUS |
| cycles | 2 |
| opcode | 0x6P |
| param  | IO |
    

| RET    ||
| ------ | --- |
| desc   | Return |
| logic  | (XP) <- M(R(X)); R(X) <- R(X)+1; IE <- 1 |
| cycles | 2 |
| opcode | 0x70 |
| param  | NONE |
    

| DIS    ||
| ------ | --- |
| desc   | Return and Disable Interrupts |
| logic  | (XP) <- M(R(X)); R(X) <- R(X)+1; IE <- 0 |
| cycles | 2 |
| opcode | 0x71 |
| param  | NONE |
    

| LDXA   ||
| ------ | --- |
| desc   | Load D via R(X) and Advance |
| logic  | D <- M(R(X)); R(X) <- R(X)+1 |
| cycles | 2 |
| opcode | 0x72 |
| param  | NONE |
    

| STXD   ||
| ------ | --- |
| desc   | Store D via R(X) and Decrement |
| logic  | M(R(X)) <- D; R(X) <- R(X)-1 |
| cycles | 2 |
| opcode | 0x73 |
| param  | NONE |
    

| ADC    ||
| ------ | --- |
| desc   | Add with Carry |
| logic  | DF,D <- M(R(X)) + D + DF |
| cycles | 2 |
| opcode | 0x74 |
| param  | NONE |
    

| SDB    ||
| ------ | --- |
| desc   | Subtract D from memory with Borrow |
| logic  | DF,D <- M(R(X)) - D - complement(DF) |
| cycles | 2 |
| opcode | 0x75 |
| param  | NONE |
    

| SHRC   ||
| ------ | --- |
| desc   | Shift D Right with Carry |
| logic  | shift D right one bit; MSB(D) <- DF ; DF <- LSB(D) |
| cycles | 2 |
| opcode | 0x76 |
| param  | NONE |
    

| SMB    ||
| ------ | --- |
| desc   | Subtract Memory from D with Borrow |
 | logic  | DF,D <- D - M(R(X)) - complement(DF) |
| cycles | 2 |
| opcode | 0x77 |
| param  | NONE |
    

| SAV    ||
| ------ | --- |
| desc   | Save T |
| logic  | M(R(X)) <- T |
| cycles | 2 |
| opcode | 0x78 |
| param  | NONE |
    

| MARK   ||
| ------ | --- |
| desc   | Save X and P in T |
| logic  | T <- XP ; M(R(2)) <- T; X <- P; R(2) <- R(2)-1 |
| cycles | 2 |
| opcode | 0x79 |
| param  | NONE |
    

| REQ    ||
| ------ | --- |
| desc   | Reset Q |
| logic  | Q <- 0 |
| cycles | 2 |
| opcode | 0x0A |
| param  | NONE |
    

| SEQ    ||
| ------ | --- |
| desc   | Set Q |
| logic  | Q <- 1 |
| cycles | 2 |
| opcode | 0x7B |
| param  | NONE |
    

| ADCI   ||
| ------ | --- |
| desc   | Add with Carry Immediate |
| logic  | DF,D <- M(R(P)) + D + DF; R(P) <- R(P)+1 |
| cycles | 2 |
| opcode | 0x7C bb |
| param  | BYTE |
    

| SDBI   ||
| ------ | --- |
| desc   | Subtract D with Borrow Immediate |
| logic  | DF,D <- M(R(P)) - D - complement(DF); R(P) <- R(P)+1 |
| cycles | 2 |
| opcode | 0x7D bb |
| param  | BYTE |
    

| SHLC   ||
| ------ | --- |
| desc   | Shift D Left with Carry |
| logic  | shift D left one bit; LSB(D) <- DF ; DF <- MSB(D) |
| cycles | 2 |
| opcode | 0x7E |
| param  | NONE |
    

| SMBI   ||
| ------ | --- |
| desc   | Subtract Memory with Borrow Immediate |
| logic  | DF,D <- D - M(R(P)) - complement(DF); R(P) <- R(P)+1 |
| cycles | 2 |
| opcode | 0x7F bb |
| param  | BYTE |
    

| GLO    ||
| ------ | --- |
| desc   | Get Low byte of Register |
| logic  | D <- R(N).0 |
| cycles | 2 |
| opcode | 0x8N | 
| param  | REGISTER |
    

| GHI    ||
| ------ | --- |
| desc   | Get High byte of Register |
| logic  | D <- R(N).1 |
| cycles | 2 |
| opcode | 0x9N |
| param  | REGISTER |
    

| PLO    ||
| ------ | --- |
| desc   | Put D in Low byte of register |
| logic  | R(N).0 <- D |
| cycles | 2 |
| opcode | 0xAN |
| param  | REGISTER |
    

| PHI    ||
| ------ | --- |
| desc   | Put D in High byte of register |
| logic  | R(N).1 <- D |
| cycles | 2 |
| opcode | 0xBN |
| param  | REGISTER |
    

| LBR    ||
| ------ | --- |
| desc   | Long Branch unconditionally |
| logic  | R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1) |
| cycles | 3 |
| opcode | 0xC0 aaaa |
| param  | WORD ADDRESS |
    

| LBQ    ||
| ------ | --- |
| desc   | Long Branch if Q is on |
| logic  | if Q==1 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1); else R(P) <- R(P)+2 |
| cycles | 3 |
| opcode | 0xC1 aaaa |
| param  | WORD ADDRESS |
    

| LBZ    ||
| ------ | --- |
| desc   | Long Branch if Zero |
| logic  | if D==0 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1); else R(P) <- R(P)+2 |
| cycles | 3 |
| opcode | 0xC2 aaaa |
| param  | WORD ADDRESS |
    

| LBDF   ||
| ------ | --- |
| desc   | Long Branch if DF is 1 |
| logic  | if DF==1 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1); else R(P) <- R(P)+2 |
| cycles | 3 |
| opcode | 0xC3 aaaa |
| param  | WORD ADDRESS |
    

| NOP    ||
| ------ | --- |
| desc   | No Operation |
| logic  | R(P) <- R(P)+1 |
| cycles | 3 |
| opcode | 0xC4 |
| param  | NONE |
    

| LSNQ   ||
| ------ | --- |
| desc   | Long Skip if Q is off |
| logic  | if Q==0 then R(P) <- R(P) + 2 |
| cycles | 3 |
| opcode | 0xC5 |
| param  | NONE |
    

| LSNZ   ||
| ------ | --- |
| desc   | Long Skip if Not Zero |
| logic  | if D!=0 then R(P) <- R(P) + 2 |
| cycles | 3 |
| opcode | 0xC6 |
| param  | NONE |
    

| LSNF   ||
| ------ | --- |
| desc   | Long Skip if DF is 0 |
| logic  | if DF==0 then R(P) <- R(P) + 2 |
| cycles | 3 |
| opcode | 0xC7 |
| param  | NONE |
    

| LSKP   ||
| ------ | --- |
| desc   | Long Skip |
| logic  | R(P) <- R(P) + 2 |
| cycles | 3 |
| opcode | 0xC8 |
| param  | NONE |
    

| LBNQ   ||
| ------ | --- |
| desc   | Long Branch if Q is off |
| logic  | if Q==0 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1); else R(P) <- R(P)+2 |
| cycles | 3 |
| opcode | 0xC9 aaaa |
| param  | WORD ADDRESS |
    

| LBNZ   ||
| ------ | --- |
| desc   | Long Branch if Not Zero |
| logic  | if D!=0 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1); else R(P) <- R(P)+2 |
| cycles | 3 |
| opcode | 0xCA aaaa |
| param  | WORD ADDRESS |
    

| LBNF   ||
| ------ | --- |
| desc   | Long Branch if DF is 0 |
| logic  | if DF==0 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1); else R(P) <- R(P)+2 |
| cycles | 3 |
| opcode | 0xCB aaaa |
| param  | WORD ADDRESS |
    

| LSIE   ||
| ------ | --- |
| desc   | Long Skip if Interrupts Enabled |
| logic  | if IE==1 then R(P) <- R(P)+2 |
| cycles | 3 |
| opcode | 0xCC |
| param  | NONE |
    

| LSQ    ||
| ------ | --- |
| desc   | Long Skip if Q is on |
| logic  | if Q==1 then R(P) <- R(P)+2 |
| cycles | 3 |
| opcode | 0xCD |
| param  | NONE |
    

| LSZ    ||
| ------ | --- |
| desc   | Long Skip if Zero |
| logic  | if D==0 then R(P) <- R(P)+2 |
| cycles | 3 |
| opcode | 0xCE |
| param  | NONE |
    

| LSDF   ||
| ------ | --- |
| desc   | Long Skip if DF is 1 |
| logic  | if DF==1 then R(P) <- R(P)+2 |
| cycles | 3 |
| opcode | 0xCF |
| param  | NONE |
    

| SEP    ||
| ------ | --- |
| desc   | Set P |
| logic  | P <- N |
| cycles | 3 |
| opcode | 0xD0 |
| param  | REGISTER |
    

| SEX    ||
| ------ | --- |
| desc   | Set X |
| logic  | X <- N |
| cycles | 2 |
| opcode | 0xEN |
| param  | REGISTER |
    

| LDX    ||
| ------ | --- |
| desc   | Load D via R(X) |
| logic  | D <- M(R(X)) |
| cycles | 2 |
| opcode | 0xF0 |
| param  | NONE |
    

| OR     ||
| ------ | --- |
| desc   | Logical OR |
| logic  | D <- M(R(X)) OR D |
| cycles | 2 |
| opcode | 0xF1 |
| param  | NONE |
    

| AND    ||
| ------ | --- |
| desc   | Logical AND |
| logic  | D <- M(R(X)) AND D |
| cycles | 2 |
| opcode | 0xF2 |
| param  | NONE |


| XOR    ||
| ------ | --- |
| desc   | Exclusive OR |
| logic  | D <- M(R(X)) XOR D |
| cycles | 2 |
| opcode | 0xF3 |
| param  | NONE |
    

| ADD    ||
| ------ | --- |
| desc   | Add |
| logic  | DF,D <- M(R(X)) + D |
| cycles | 2 |
| opcode | 0xF4 |
| param  | NONE |
    

| SD     ||
| ------ | --- |
| desc   | Subtract D from memory |
| logic  | DF,D <- M(R(X)) - D |
| cycles | 2 |
| opcode | 0xF5 |
| param  | NONE |
    

| SHR    ||
| ------ | --- |
| desc   | Shift D Right |
| logic  | Shift D right one bit; MSB(D) <- 0; DF <- LSB(D) |
| cycles | 2 |
| opcode | 0xF6 |
| param  | NONE |
    

| SM     ||
| ------ | --- |
| desc   | Subtract Memory from D |
| logic  | DF,D <- D - M(R(X)) |
| cycles | 2 |
| opcode | 0xF7 |
| param  | NONE |
    

| LDI    ||
| ------ | --- |
| desc   | Load D Immediate |
| logic  | D <-M(R(P)); R(P) <- R(P)+1 |
| cycles | 2 |
| opcode | 0xF8 bb |
| param  | BYTE |
    

| ORI    ||
| ------ | --- |
| desc   | OR Immediate |
| logic  | D <- M(R(P)) OR D; R(P) <- R(P)+1 |
| cycles | 2 |
| opcode | 0xF9 bb |
| param  | BYTE |
    

| ANI    ||
| ------ | --- |
| desc   | AND Immediate |
| logic  | D <- M(R(P)) AND D; R(P) <- R(P)+1 |
| cycles | 2 |
| opcode | 0xFA bb |
| param  | BYTE |
    

| XRI    ||
| ------ | --- |
| desc   | Exclusive OR Immediate |
| logic  | D <- M(R(P)) XOR D; R(P) <- R(P)+1 |
| cycles | 2 |
| opcode | 0xFB bb |
| param  | BYTE |
    

| ADI    ||
| ------ | --- |
| desc   | Add Immediate |
| logic  | DF,D <- M(R(P)) + D; R(P) <- R(P)+1 |
| cycles | 2 |
| opcode | 0xFC bb |
| param  | BYTE |
    

| SDI    ||
| ------ | --- |
| desc   | Subtract D from memory Immediate |
| logic  | DF,D <- M(R(P)) - D; R(P) <- R(P)+1 |
| cycles | 2 |
| opcode | 0xFD bb |
| param  | BYTE |
    

| SHL    ||
| ------ | --- |
| desc   | Shift D Left |
| logic  | Shift left one bit; LSB(D) <- 0 ; DF <- MSB(D) |
| cycles | 2 |
| opcode | 0xFE |
| param  | NONE |
    

| SMI    ||
| ------ | --- |
| desc   | Subtract Memory from D Immediate |
| logic  | DF,D <- D - M(R(P)); R(P) <- R(P)+1 |
| cycles | 2 |
| opcode | 0xFF |
| param  | BYTE |
    

