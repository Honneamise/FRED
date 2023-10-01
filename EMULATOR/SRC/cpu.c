#include "funcs.h"
#include "memory.h"
#include "bus.h"
#include "io.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef struct CPU
{
	uint16_t REGS[16];//16 registers, 16bit each

	uint8_t D;//8bit accumulator

	uint8_t DF;//1bit carry

	uint8_t P;//4bit ptr to the register holding the PC

	uint8_t X;//4bit ptr to the register holding the SP

	uint8_t T;//8bit used during interrupts to preserve X and P

} CPU;

static CPU *cpu = NULL;

/*********/
static void cpu_dump(void)
{
    printf("D: 0x%02X\n", cpu->D);

	printf("DF: %d\n", (int)cpu->DF);

	printf("P: %d\n", (int)cpu->P);

	printf("X: %d\n", (int)cpu->X);

	printf("T: 0x%02X\n", cpu->T);

    for(int i=0;i<16;i++)
    {
        printf("R%d: 0x%04X\n", i, cpu->REGS[i]);
    }
}

/*********/
//R(P) <- R(P)-1; Bus <- M(R(0)) until an interrupt or DMA request
//NOTE so far we dont have any DMA, so it act like an HALT
static uint8_t cpu_IDL(void)
{
	cpu->REGS[cpu->P]--;

	uint8_t byte = memory_read(cpu->REGS[0]);

	bus_write(byte);

    return 2;
}

/*********/
//D <- M(R(N)) for N != 0
static uint8_t cpu_LDN(uint8_t op)
{
	cpu->D = memory_read(cpu->REGS[op & 0x0F]);

    return 2;
}

/*********/
//R(N) <- R(N)+1
static uint8_t cpu_INC(uint8_t op)
{
	cpu->REGS[op & 0x0F]++;

    return 2;
}

/*********/
//R(N) <- R(N)-1
static uint8_t cpu_DEC(uint8_t op)
{
	cpu->REGS[op & 0x0F]--;

    return 2;
}

/*********/
//R(P).0 <- M(R(P))
static uint8_t cpu_BR(void)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;

    return 2;
}

/*********/
//if Q==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_BQ(void)
{
	if (io_get(Q) == 1)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	}

    return 2;
}

/*********/
//if D==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_BZ(void)
{
	if (cpu->D == 0)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	}

    return 2;
}

/*********/
//if DF==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_BDF(void)
{
	if (cpu->DF == 1)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	}

    return 2;
}

/*********/
//if EF1==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_B1(void)
{
    if (io_get(EF1) == 1)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	}

    return 2;
}

/*********/
//if EF2==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_B2(void)
{
    if (io_get(EF2) == 1)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	}

    return 2;
}

/*********/
//if EF3==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_B3(void)
{
    if (io_get(EF3) == 1)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	}

    return 2;
}

/*********/
//if EF4==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_B4(void)
{
    if (io_get(EF4) == 1)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	}

    return 2;
}

/*********/
//R(P) <- R(P)+1;
static uint8_t cpu_SKP(void)
{
	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//if Q==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_BNQ(void)
{
	if (io_get(Q) == 0)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	}

    return 2;
}

/*********/
//if D!=0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_BNZ(void)
{	
	if (cpu->D != 0)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	}

    return 2;
}

/*********/
//if DF!=0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_BNF(void)
{	 
	if (cpu->DF != 0)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	}

    return 2;
}

/*********/
//if EF1==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) + 1
static uint8_t cpu_BN1(void)
{
    if(io_get(EF1) == 0)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	}

    return 2;
}

/*********/
//if EF2==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) + 1
static uint8_t cpu_BN2(void)
{
    if(io_get(EF2) == 0)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	}

    return 2;
}

/*********/
//if EF3==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) + 1
static uint8_t cpu_BN3(void)
{
    if(io_get(EF3) == 0)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	}

    return 2;
}

/*********/
//if EF4==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) + 1
static uint8_t cpu_BN4(void)
{
    if(io_get(EF4) == 0)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	}

    return 2;
}

/*********/
//D <- M(R(N)); R(N) <- R(N)+1
static uint8_t cpu_LDA(uint8_t op)
{
    cpu->D = memory_read(cpu->REGS[op & 0x0F]);

	cpu->REGS[op & 0x0F]++;

    return 2;
}

/*********/
//M(R(N)) <- D
static uint8_t cpu_STR(uint8_t op)
{
    memory_write(cpu->REGS[op & 0x0F], cpu->D);

    return 2;
}

/*********/
//R(X) <- R(X)+1
static uint8_t cpu_IRX(void)
{
	cpu->REGS[cpu->X]++;

    return 2;
}

/*********/
//BUS <- M(R(X)) ; R(X) <- R(X)+1
static uint8_t cpu_OUT(uint8_t op)
{
    uint8_t device_id = op & 0x0F;

	uint8_t byte = memory_read(cpu->REGS[cpu->X]);
	
	bus_write(byte);
	
    io_activate_device(device_id, IO_OUT);

    cpu->REGS[cpu->X]++;
    
    return 2;
}

/*********/
//M(R(X) <- BUS; D <- BUS
static uint8_t cpu_INP(uint8_t op)
{
    uint8_t device_id = (op & 0x0F) - 0x08;

    io_activate_device(device_id, IO_INP);

    uint8_t byte = bus_read(); 

	memory_write(cpu->REGS[cpu->X], byte);

	cpu->D = byte;

    return 2;
}

/*********/
//(X,P) <- M(R(X)); R(X) <- R(X)+1; IE <- 1
static uint8_t cpu_RET(void) 
{
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	cpu->X = (byte & 0xF0) >> 4;
	cpu->P = byte & 0x0F;

	cpu->REGS[cpu->X]++;

    io_set(IE, 1);

    return 2;
}

/*********/
//(X,P) <- M(R(X)); R(X) <- R(X)+1; IE <- 0
static uint8_t cpu_DIS(void)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	cpu->X = (byte & 0xF0) >> 4;
	cpu->P = byte & 0x0F;

	cpu->REGS[cpu->X]++;

	io_set(IE, 0);

    return 2;
}

/*********/
//D <- M(R(X)); R(X) <- R(X)+1
static uint8_t cpu_LDXA(void)
{
	cpu->D = memory_read(cpu->REGS[cpu->X]);

	cpu->REGS[cpu->X]++;

    return 2;
}

/*********/
//M(R(X)) <- D; R(X) <- R(X)-1
static uint8_t cpu_STXD(void)
{
    memory_write(cpu->REGS[cpu->X], cpu->D);

	cpu->REGS[cpu->X]--;

    return 2;
}

/*********/
//DF,D <- M(R(X)) + D + DF
static uint8_t cpu_ADC(void)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	if ((byte + cpu->D + cpu->DF) > 0xFF) { cpu->DF = 1; }
	else { cpu->DF = 0; }

	cpu->D = (uint8_t)( byte + cpu->D + cpu->DF );

    return 2;
}

/*********/
//D,DF <- M(R(X)) - D - complement(DF)
static uint8_t cpu_SDB(void)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);
    
    uint8_t compDF = !cpu->DF;

	if (byte - cpu->D - compDF < 0) { cpu->DF = 1; }
	else { cpu->DF = 0; }

	cpu->D = (uint8_t)(byte - cpu->D - compDF);

    return 2;
}

/*********/
//shift D right one bit; MSB(D) <- DF ; DF <- LSB(D)
static uint8_t cpu_SHRC(void)
{
	uint8_t lsb = cpu->D & 0x01;
	
	cpu->D = (uint8_t)(cpu->D >> 1);

	cpu->D += (uint8_t)(cpu->DF << 7);

	cpu->DF = lsb;

    return 2;
}

/*********/
//DF,D <- D - M(R(X)) - complement(DF)
static uint8_t cpu_SMB(void)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	uint8_t compDF = !cpu->DF;
	
	if (cpu->D - byte - compDF < 0 ) { cpu->DF = 1; }
	else { cpu->DF = 0; }
	
	cpu->D = (uint8_t)(cpu->D - byte - compDF);

    return 2;
}

/*********/
//M(R(X)) <- T
static uint8_t cpu_SAV(void)
{
	memory_write(cpu->REGS[cpu->X], cpu->T);

    return 2;
}

/*********/
//T <- X,P ; M(R(2) <- T; X <- P; R(2) <- R(2)-1
static uint8_t cpu_MARK(void)
{
	cpu->T = ((uint8_t)cpu->X << 4) + cpu->P;

	memory_write(cpu->REGS[2], cpu->T);

	cpu->X = cpu->P;

	cpu->REGS[2]--;

    return 2;
}

/*********/
//Q <- 0
static uint8_t cpu_REQ(void)
{
    io_set(Q, 0);

    return 2;
}

/*********/
//Q <- 1
static uint8_t cpu_SEQ(void)
{
	io_set(Q, 1);

    return 2;
}

/*********/
//DF, D <- M(R(P)) + D + DF; R(P) <- R(P)+1
static uint8_t cpu_ADCI(void)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	if ((byte + cpu->D + cpu->DF) > 0xFF) { cpu->DF = 1; }
	else { cpu->DF = 0; }

	cpu->D = (uint8_t)(byte + cpu->D + cpu->DF);

	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//DF,D <- M(R(P)) - D - complement(DF); R(P) <- R(P)+1
static uint8_t cpu_SDBI(void)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	uint8_t compDF = !cpu->DF;

	if (byte - cpu->D - compDF < 0) { cpu->DF = 1; }
	else { cpu->DF = 0; }

	cpu->D = (uint8_t)(byte - cpu->D - compDF);

	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//shift D left one bit; LSB(D) <- DF ; DF <- MSB(D)
static uint8_t cpu_SHLC(void)
{
	uint8_t msb = ((uint8_t)(cpu->D >> 7)) % 0x01;

	cpu->D = (uint8_t)(cpu->D << 1);

	cpu->D += cpu->DF;

	cpu->DF = msb;

    return 2;
}

/*********/
//DF,D <- D - M(R(P)) - complement(DF); R(P) <- R(P)+1
static uint8_t cpu_SMBI(void)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	uint8_t compDF = !cpu->DF;

	if (cpu->D - byte - compDF < 0) { cpu->DF = 1; }
	else { cpu->DF = 0; }

	cpu->D = (uint8_t)(cpu->D - byte - compDF);

	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
// D <- R(N).0
static uint8_t cpu_GLO(uint8_t op)
{
	cpu->D = (uint8_t)(cpu->REGS[op & 0x0F] & 0x00FF);

    return 2;
}

/*********/
// D <- R(N).1
static uint8_t cpu_GHI(uint8_t op)
{
	cpu->D = (uint8_t)(cpu->REGS[op & 0x0F] >> 8);

    return 2;
}

/*********/
//R(N).0 <- D
static uint8_t cpu_PLO(uint8_t op)
{
	cpu->REGS[op & 0x0F] = (cpu->REGS[op & 0x0F] & 0xFF00) + cpu->D;

    return 2;
}

/*********/
//R(N).1 <- D
static uint8_t cpu_PHI(uint8_t op)
{
	cpu->REGS[op & 0x0F] = (cpu->REGS[op & 0x0F] & 0x00FF) + (uint16_t)(cpu->D << 8) ;

    return 2;
}

/*********/
//R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1);
static uint8_t cpu_LBR(void)
{
	uint8_t high = memory_read(cpu->REGS[cpu->P]);
	uint8_t low = memory_read(cpu->REGS[cpu->P] + 1);

	cpu->REGS[cpu->P] = (high << 8) + low;

    return 3;
}

/*********/
//if Q==1 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1)
//else R(P) <- R(P)+2
static uint8_t cpu_LBQ(void)
{
	if ( io_get(Q) == 1)
	{
		uint8_t high = memory_read(cpu->REGS[cpu->P]);
		uint8_t low = memory_read(cpu->REGS[cpu->P] + 1);

		cpu->REGS[cpu->P] = (high << 8) + low;
	}
	else
	{
		cpu->REGS[cpu->P] += 2;
	}

    return 3;
}

/*********/
//if D==0 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1);
//else R(P) <- R(P)+2
static uint8_t cpu_LBZ(void)
{
	if (cpu->D == 0)
	{
		uint8_t high = memory_read(cpu->REGS[cpu->P]);
		uint8_t low = memory_read(cpu->REGS[cpu->P] + 1);

		cpu->REGS[cpu->P] = (high << 8) + low;
	}
	else
	{
		cpu->REGS[cpu->P] += 2;
	}

    return 3;
}

/*********/
//if DF==1 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1);
//else R(P) <- R(P)+2	
static uint8_t cpu_LBDF(void)
{
	if (cpu->DF == 1)
	{
		uint8_t high = memory_read(cpu->REGS[cpu->P]);
		uint8_t low = memory_read(cpu->REGS[cpu->P] + 1);

		cpu->REGS[cpu->P] = (high << 8) + low;
	}
	else
	{
		cpu->REGS[cpu->P] += 2;
	}

    return 3;
}

/*********/
//R(P) <- R(P)+1
static uint8_t cpu_NOP(void)
{	
	cpu->REGS[cpu->P]++;

    return 3;
}

/*********/
//if Q==0 then R(P) <- R(P) + 2
static uint8_t cpu_LSNQ(void)
{	
	if (io_get(Q) == 0)
	{
		cpu->REGS[cpu->P] += 2;
	}

    return 3;
}

/*********/
//if D!=0 then R(P) <- R(P) + 2
static uint8_t cpu_LSNZ(void)
{
	if (cpu->D != 0)
	{
		cpu->REGS[cpu->P] += 2;
	}

    return 3;
}

/*********/
//if DF==0 then R(P) <- R(P) + 2
static uint8_t cpu_LSNF(void)
{	
	if (cpu->DF == 0)
	{
		cpu->REGS[cpu->P] += 2;
	}

    return 3;
}

/*********/
//R(P) <- R(P) + 2
static uint8_t cpu_LSKP(void)
{	
	cpu->REGS[cpu->P] += 2;

    return 3;
}

/*********/
//if Q==0 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1)
//else R(P) <- R(P)+2
static uint8_t cpu_LBNQ(void)
{	
	if (io_get(Q) == 0)
	{
		uint8_t high = memory_read(cpu->REGS[cpu->P]);
		uint8_t low = memory_read(cpu->REGS[cpu->P] + 1);

		cpu->REGS[cpu->P] = (high << 8) + low;
	}
	else
	{
		cpu->REGS[cpu->P] += 2;
	}

    return 3;
}

/*********/
//if D!=0 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1);
//else R(P) <- R(P)+2
static uint8_t cpu_LBNZ(void)
{
	if (cpu->D != 0)
	{
		uint8_t high = memory_read(cpu->REGS[cpu->P]);
		uint8_t low = memory_read(cpu->REGS[cpu->P] + 1);

		cpu->REGS[cpu->P] = (high << 8) + low;
	}
	else
	{
		cpu->REGS[cpu->P] += 2;
	}

    return 3;
}

/*********/
//if DF==0 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1);
//else R(P) <- R(P)+2
static uint8_t cpu_LBNF(void)
{
	if (cpu->DF == 0)
	{
		uint8_t high = memory_read(cpu->REGS[cpu->P]);
		uint8_t low = memory_read(cpu->REGS[cpu->P] + 1);

		cpu->REGS[cpu->P] = (high << 8) + low;
	}
	else
	{
		cpu->REGS[cpu->P] += 2;
	}

    return 3;
}

/*********/
//if IE==1 then R(P) <- R(P)+2
static uint8_t cpu_LSIE(void)
{
	if (io_get(IE) == 1)
	{
		cpu->REGS[cpu->P] += 2;
	}

    return 3;
}

/*********/
//if Q==1 then R(P) <- R(P)+2
static uint8_t cpu_LSQ(void)
{
	if (io_get(Q) == 1)
	{
		cpu->REGS[cpu->P] += 2;
	}

    return 3;
}

/*********/
//if D==0 then R(P) <- R(P)+2
static uint8_t cpu_LSZ(void)
{
	if (cpu->D == 0)
	{
		cpu->REGS[cpu->P] += 2;
	}

    return 3;
}

/*********/
//if DF==1 then R(P) <- R(P)+2
static uint8_t cpu_LSDF(void)
{
	if (cpu->DF == 1)
	{
		cpu->REGS[cpu->P] += 2;
	}

    return 3;
}

/*********/
//P <- N
static uint8_t cpu_SEP(uint8_t op)
{
	cpu->P = op & 0x0F;

    return 2;
}

/*********/
//X <- N
static uint8_t cpu_SEX(uint8_t op)
{	
	cpu->X = op & 0x0F;

    return 2;
}

/*********/
//D <- M(R(X))
static uint8_t cpu_LDX(void)
{	
	cpu->D = memory_read(cpu->REGS[cpu->X]);

    return 2;
}

/*********/
//D <- M(R(X)) OR D
static uint8_t cpu_OR(void)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	cpu->D = byte | cpu->D;

    return 2;
}

/*********/
//D <- M(R(X)) AND D
static uint8_t cpu_AND(void)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	cpu->D = byte & cpu->D;

    return 2;
}

/*********/
//D <- M(R(X)) XOR D
static uint8_t cpu_XOR(void)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	cpu->D = byte ^ cpu->D;

    return 2;
}

/*********/
//DF,D <- M(R(X)) + D
static uint8_t cpu_ADD(void)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	if ((byte + cpu->D) > 0xFF) { cpu->DF = 1; }
	else { cpu->DF = 0; }

	cpu->D = (uint8_t)(byte+ cpu->D);

    return 2;
}

/*********/
//DF,D <- M(R(X)) - D
static uint8_t cpu_SD(void)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	if ( cpu->D > byte ) { cpu->DF = 1; }
	else { cpu->DF = 0; }

	cpu->D = (uint8_t)(byte - cpu->D);

    return 2;
}

/*********/
//Shift D right one bit; MSB(D) <- 0; DF <- LSB(D)
static uint8_t cpu_SHR(void)
{	
	cpu->DF = cpu->D & 0x01;
	cpu->D = cpu->D >> 1;

    return 2;
}

/*********/
//DF,D <- D-M(R(X))
static uint8_t cpu_SM(void)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	if (byte > cpu->D) { cpu->DF = 1; }
	else { cpu->DF = 0; }

	cpu->D = (uint8_t)(cpu->D - byte);

    return 2;
}

/*********/
//D <-M(R(P)); R(P) <- R(P)+1
static uint8_t cpu_LDI(void)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	cpu->D = byte;

	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//D <- M(R(P)) OR D; R(P) <- R(P)+1
static uint8_t cpu_ORI(void)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	cpu->D = byte | cpu->D;

	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//D <- M(R(P)) AND D; R(P) <- R(P)+1
static uint8_t cpu_ANI(void)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	cpu->D = byte & cpu->D;

	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//D <- M(R(P)) XOR D; R(P) <- R(P)+1
static uint8_t cpu_XRI(void)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	cpu->D = byte ^ cpu->D;

	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//DF,D <- M(R(P)) + D; R(P) <- R(P)+1
static uint8_t cpu_ADI(void)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	if ((cpu->D + byte) > 0xFF) { cpu->DF = 1; }
	else { cpu->DF = 0; }

	cpu->D = (uint8_t)(byte + cpu->D);

	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//DF,D <- M(R(P)) - D; R(P) <- R(P)+1
static uint8_t cpu_SDI(void)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	if ( cpu->D > byte ) { cpu->DF = 1; }
	else { cpu->DF = 0; }

	cpu->D = (uint8_t)(byte - cpu->D);

	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//Shift left one bit; LSB(D) <- 0 ; DF <- MSB(D)
static uint8_t cpu_SHL(void)
{	
	cpu->DF = ((uint8_t)cpu->D >> 7) & 0x01;

	cpu->D = (uint8_t)cpu->D << 1;

    return 2;
}

/*********/
//DF,D <- D - M(R(P)); R(P) <- R(P)+1
static uint8_t cpu_SMI(void)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	if ( byte > cpu->D ) { cpu->DF = 1; }
	else { cpu->DF = 0; }

	cpu->D = (uint8_t)(cpu->D - byte);

	cpu->REGS[cpu->P]++;

    return 2;
}

/**********/
static uint8_t cpu_execute0(uint8_t op)
{
	if((op & 0x0F)==0)
	{
		return cpu_IDL();
	}
	else
	{
		return cpu_LDN(op);
	}
}

/**********/
static uint8_t cpu_execute1(uint8_t op)
{
	return cpu_INC(op);
}

/**********/
static uint8_t cpu_execute2(uint8_t op)
{
	return cpu_DEC(op);
}

/**********/
static uint8_t cpu_execute3(uint8_t op)
{
	switch (op & 0x0F)
	{
        case 0x00: return cpu_BR();
        case 0x01: return cpu_BQ();
        case 0x02: return cpu_BZ();
        case 0x03: return cpu_BDF();

        case 0x04: return cpu_B1();
        case 0x05: return cpu_B2();
        case 0x06: return cpu_B3();
        case 0x07: return cpu_B4();

        case 0x08: return cpu_SKP();
        case 0x09: return cpu_BNQ();
        case 0x0A: return cpu_BNZ();
        case 0x0B: return cpu_BNF();

        case 0x0C: return cpu_BN1();
        case 0x0D: return cpu_BN2();
        case 0x0E: return cpu_BN3();
        case 0x0F: return cpu_BN4();
	}

	return 0;
}

/**********/
static uint8_t cpu_execute4(uint8_t op)
{
	return cpu_LDA(op);
}

/**********/
static uint8_t cpu_execute5(uint8_t op)
{
	return cpu_STR(op);
}

/**********/
static uint8_t cpu_execute6(uint8_t op)
{
    switch(op & 0x0F)
    {
        case 0x00: return cpu_IRX();

        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07: return cpu_OUT(op);

        //case 0x08: invalid opcode

        case 0x09:
        case 0x0A:
        case 0x0B:
        case 0x0C:
        case 0x0D:
        case 0x0E:
        case 0x0F: return cpu_INP(op);
    }

	return 0;
}

/**********/
static uint8_t cpu_execute7(uint8_t op)
{
	switch (op & 0x0F)
	{
        case 0x00: return cpu_RET();
        case 0x01: return cpu_DIS();
        case 0x02: return cpu_LDXA();
        case 0x03: return cpu_STXD();
        case 0x04: return cpu_ADC();
        case 0x05: return cpu_SDB();
        case 0x06: return cpu_SHRC();
        case 0x07: return cpu_SMB();
        case 0x08: return cpu_SAV();
        case 0x09: return cpu_MARK();
        case 0x0A: return cpu_REQ();
        case 0x0B: return cpu_SEQ();
        case 0x0C: return cpu_ADCI();
        case 0x0D: return cpu_SDBI();
        case 0x0E: return cpu_SHLC();
        case 0x0F: return cpu_SMBI(); 
	}

	return 0;
}

/**********/
static uint8_t cpu_execute8(uint8_t op)
{
	return cpu_GLO(op);
}

/**********/
static uint8_t cpu_execute9(uint8_t op)
{
	return cpu_GHI(op);
}

/**********/
static uint8_t cpu_executeA(uint8_t op)
{
	return cpu_PLO(op);
}

/**********/
static uint8_t cpu_executeB(uint8_t op)
{
	return cpu_PHI(op);
}

/**********/
static uint8_t cpu_executeC(uint8_t op)
{
	switch(op & 0x0F)
	{
        case 0x00: return cpu_LBR();
        case 0x01: return cpu_LBQ();
        case 0x02: return cpu_LBZ();
        case 0x03: return cpu_LBDF();
        case 0x04: return cpu_NOP();
        case 0x05: return cpu_LSNQ();
        case 0x06: return cpu_LSNZ();
        case 0x07: return cpu_LSNF();
        case 0x08: return cpu_LSKP();
        case 0x09: return cpu_LBNQ();
        case 0x0A: return cpu_LBNZ();
        case 0x0B: return cpu_LBNF();
        case 0x0C: return cpu_LSIE();
        case 0x0D: return cpu_LSQ();
        case 0x0E: return cpu_LSZ();
        case 0x0F: return cpu_LSDF();
        
	}

	return 0;
}

/**********/
static uint8_t cpu_executeD(uint8_t op)
{
	return cpu_SEP(op);
}

/**********/
static uint8_t cpu_executeE(uint8_t op)
{
	return cpu_SEX(op);
}

/**********/
static uint8_t cpu_executeF(uint8_t op)
{
	switch (op & 0x0F)
	{
        case 0x00: return cpu_LDX();
        case 0x01: return cpu_OR();
        case 0x02: return cpu_AND();
        case 0x03: return cpu_XOR();
        case 0x04: return cpu_ADD();
        case 0x05: return cpu_SD();
        case 0x06: return cpu_SHR();
        case 0x07: return cpu_SM();
        case 0x08: return cpu_LDI();
        case 0x09: return cpu_ORI();
        case 0x0A: return cpu_ANI();
        case 0x0B: return cpu_XRI();
        case 0x0C: return cpu_ADI();
        case 0x0D: return cpu_SDI();
        case 0x0E: return cpu_SHL();
        case 0x0F: return cpu_SMI();
	}

	return 0;
}

/**********/
static uint8_t cpu_execute(uint8_t op)
{
    //opcode 0x68 is not part of ISA, useing it just for debug
    if(op == 0x68) 
    {   
        printf("***CPU DUMP***\n");
        cpu_dump();
        io_dump();
    }

    switch(op & 0xF0)
	{
        case 0x00: return cpu_execute0(op); 
        case 0x10: return cpu_execute1(op); 
        case 0x20: return cpu_execute2(op); 
        case 0x30: return cpu_execute3(op);
        case 0x40: return cpu_execute4(op);
        case 0x50: return cpu_execute5(op);
        case 0x60: return cpu_execute6(op);
        case 0x70: return cpu_execute7(op);
        case 0x80: return cpu_execute8(op);
        case 0x90: return cpu_execute9(op);
        case 0xA0: return cpu_executeA(op);
        case 0xB0: return cpu_executeB(op);
        case 0xC0: return cpu_executeC(op);
        case 0xD0: return cpu_executeD(op);
        case 0xE0: return cpu_executeE(op);
        case 0xF0: return cpu_executeF(op);
	}

	return 0;
}

/**********/
void cpu_init(void)
{
    cpu = alloc(1, sizeof(CPU));

    //set R0 as program counter and make it point to first byte of program
    cpu->P = 0x00;
    cpu->REGS[cpu->P] = PROGRAM_BASE;

}

/**********/
void cpu_close(void)
{
    free(cpu);
}

/**********/
uint8_t cpu_run(void)
{
    //NOTE : DMA SHOULD NOT GO HERE
    // if (dma) { execute dma and return value ??? }

    //read instruction opcode
    uint8_t op = memory_read(cpu->REGS[cpu->P]);

    //increase program counter
	cpu->REGS[cpu->P]++;

    //decode and execute instruction
    return cpu_execute(op) * 8;

}