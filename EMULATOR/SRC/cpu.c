#include "funcs.h"
#include "memory.h"
#include "bus.h"
#include "io.h"
#include "device.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>

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
static void cpu_dump()
{
    printf("D: 0x%02X\n", cpu->D);

	printf("DF: %d\n", (int)cpu->DF);

	printf("P: %d\n", (int)cpu->P);

	printf("X: %d\n", (int)cpu->X);

	printf("T: 0x%02X\n", cpu->T);

    for(int i=0;i<16;i++)
    {
        printf("R%d: 0x%04X\n", i, cpu->REGS[i]);
    };
}

/*********/
//R(P) <- R(P)-1; Bus <- M(R(0)) until an interrupt or DMA request
static uint8_t cpu_IDL(uint8_t op)
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
static uint8_t cpu_BR(uint8_t op)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;

    return 2;
}

/*********/
//if Q==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_BQ(uint8_t op)
{
	if (io_get(Q) == 1)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	};

    return 2;
}

/*********/
//if D==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_BZ(uint8_t op)
{
	if (cpu->D == 0)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	};

    return 2;
}

/*********/
//if DF==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_BDF(uint8_t op)
{
	if (cpu->DF == 1)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	};

    return 2;
}

/*********/
//if EF1==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_B1(uint8_t op)
{
    if (io_get(EF1) == 1)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	};

    return 2;
}

/*********/
//if EF2==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_B2(uint8_t op)
{
    if (io_get(EF2) == 1)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	};

    return 2;
}

/*********/
//if EF3==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_B3(uint8_t op)
{
    if (io_get(EF3) == 1)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	};

    return 2;
}

/*********/
//if EF4==1 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_B4(uint8_t op)
{
    if (io_get(EF4) == 1)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	};

    return 2;
}

/*********/
//R(P) <- R(P)+1;
static uint8_t cpu_SKP(uint8_t op)
{
	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//if Q==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_BNQ(uint8_t op)
{
	if (io_get(Q) == 0)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	};

    return 2;
}

/*********/
//if D!=0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_BNZ(uint8_t op)
{	
	if (cpu->D != 0)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	};

    return 2;
}

/*********/
//if DF!=0 then R(P).0 <- M(R(P)) else R(P) <- R(P) +1
static uint8_t cpu_BNF(uint8_t op)
{	
	if (cpu->DF != 0)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	};

    return 2;
}

/*********/
//if EF1==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) + 1
static uint8_t cpu_BN1(uint8_t op)
{
    if(io_get(EF1) == 0)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	};

    return 2;
}

/*********/
//if EF2==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) + 1
static uint8_t cpu_BN2(uint8_t op)
{
    if(io_get(EF2) == 0)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	};

    return 2;
}

/*********/
//if EF3==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) + 1
static uint8_t cpu_BN3(uint8_t op)
{
    if(io_get(EF3) == 0)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	};

    return 2;
}

/*********/
//if EF4==0 then R(P).0 <- M(R(P)) else R(P) <- R(P) + 1
static uint8_t cpu_BN4(uint8_t op)
{
    if(io_get(EF4) == 0)
	{
		uint8_t byte = memory_read(cpu->REGS[cpu->P]);

		cpu->REGS[cpu->P] = (cpu->REGS[cpu->P] & 0xFF00) + byte;
	}
	else
	{
		cpu->REGS[cpu->P]++;
	};

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
static uint8_t cpu_IRX(uint8_t op)
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
	
    device_activate(device_id);

    cpu->REGS[cpu->X]++;
    
    return 2;
}

/*********/
//M(R(X) <- BUS; D <- BUS
static uint8_t cpu_INP(uint8_t op)
{
    uint8_t device_id = (op & 0x0F) - 0x08;

    device_activate(device_id);

    uint8_t byte = bus_read(); 

	memory_write(cpu->REGS[cpu->X], byte);

	cpu->D = byte;

    return 2;
}

/*********/
//(X,P) <- M(R(X)); R(X) <- R(X)+1; IE <- 1
static uint8_t cpu_RET(uint8_t op) 
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
static uint8_t cpu_DIS(uint8_t op)
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
static uint8_t cpu_LDXA(uint8_t op)
{
	cpu->D = memory_read(cpu->REGS[cpu->X]);

	cpu->REGS[cpu->X]++;

    return 2;
}

/*********/
//M(R(X)) <- D; R(X) <- R(X)-1
static uint8_t cpu_STXD(uint8_t op)
{
    memory_write(cpu->REGS[cpu->X], cpu->D);

	cpu->REGS[cpu->X]--;

    return 2;
}

/*********/
//DF,D <- M(R(X)) + D + DF
static uint8_t cpu_ADC(uint8_t op)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	if ((byte + cpu->D + cpu->DF) > 0xFF) { cpu->DF = 1; }
	else { cpu->DF = 0; };

	cpu->D = (uint8_t)( byte + cpu->D + cpu->DF );

    return 2;
}

/*********/
//D,DF <- M(R(X)) - D - complement(DF)
static uint8_t cpu_SDB(uint8_t op)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);
    
    uint8_t compDF = !cpu->DF;

	if (byte - cpu->D - compDF < 0) { cpu->DF = 1; }
	else { cpu->DF = 0; };

	cpu->D = (uint8_t)(byte - cpu->D - compDF);

    return 2;
}

/*********/
//shift D right one bit; MSB(D) <- DF ; DF <- LSB(D)
static uint8_t cpu_SHRC(uint8_t op)
{
	uint8_t lsb = cpu->D & 0x01;
	
	cpu->D = (uint8_t)(cpu->D >> 1);

	cpu->D += (uint8_t)(cpu->DF << 7);

	cpu->DF = lsb;

    return 2;
}

/*********/
//DF,D <- D - M(R(X)) - complement(DF)
static uint8_t cpu_SMB(uint8_t op)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	uint8_t compDF = !cpu->DF;
	
	if (cpu->D - byte - compDF < 0 ) { cpu->DF = 1; }
	else { cpu->DF = 0; };
	
	cpu->D = (uint8_t)(cpu->D - byte - compDF);

    return 2;
}

/*********/
//M(R(X)) <- T
static uint8_t cpu_SAV(uint8_t op)
{
	memory_write(cpu->REGS[cpu->X], cpu->T);

    return 2;
}

/*********/
//T <- X,P ; M(R(2) <- T; X <- P; R(2) <- R(2)-1
static uint8_t cpu_MARK(uint8_t op)
{
	cpu->T = ((uint8_t)cpu->X << 4) + cpu->P;

	memory_write(cpu->REGS[2], cpu->T);

	cpu->X = cpu->P;

	cpu->REGS[2]--;

    return 2;
}

/*********/
//Q <- 0
static uint8_t cpu_REQ(uint8_t op)
{
    io_set(Q, 0);

    return 2;
}

/*********/
//Q <- 1
static uint8_t cpu_SEQ(uint8_t op)
{
	io_set(Q, 1);

    return 2;
}

/*********/
//DF, D <- M(R(P)) + D + DF; R(P) <- R(P)+1
static uint8_t cpu_ADCI(uint8_t op)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	if ((byte + cpu->D + cpu->DF) > 0xFF) { cpu->DF = 1; }
	else { cpu->DF = 0; };

	cpu->D = (uint8_t)(byte + cpu->D + cpu->DF);

	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//DF,D <- M(R(P)) - D - complement(DF); R(P) <- R(P)+1
static uint8_t cpu_SDBI(uint8_t op)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	uint8_t compDF = !cpu->DF;

	if (byte - cpu->D - compDF < 0) { cpu->DF = 1; }
	else { cpu->DF = 0; };

	cpu->D = (uint8_t)(byte - cpu->D - compDF);

	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//shift D left one bit; LSB(D) <- DF ; DF <- MSB(D)
static uint8_t cpu_SHLC(uint8_t op)
{
	uint8_t msb = ((uint8_t)(cpu->D >> 7)) % 0x01;

	cpu->D = (uint8_t)(cpu->D << 1);

	cpu->D += cpu->DF;

	cpu->DF = msb;

    return 2;
}

/*********/
//DF,D <- D - M(R(P)) - complement(DF); R(P) <- R(P)+1
static uint8_t cpu_SMBI(uint8_t op)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	uint8_t compDF = !cpu->DF;

	if (cpu->D - byte - compDF < 0) { cpu->DF = 1; }
	else { cpu->DF = 0; };

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
static uint8_t cpu_LBR(uint8_t op)
{
	uint8_t high = memory_read(cpu->REGS[cpu->P]);
	uint8_t low = memory_read(cpu->REGS[cpu->P] + 1);

	cpu->REGS[cpu->P] = (high << 8) + low;

    return 3;
}

/*********/
//if Q==1 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1)
//else R(P) <- R(P)+2
static uint8_t cpu_LBQ(uint8_t op)
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
	};

    return 3;
}

/*********/
//if D==0 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1);
//else R(P) <- R(P)+2
static uint8_t cpu_LBZ(uint8_t op)
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
	};

    return 3;
}

/*********/
//if DF==1 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1);
//else R(P) <- R(P)+2	
static uint8_t cpu_LBDF(uint8_t op)
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
	};

    return 3;
}

/*********/
//R(P) <- R(P)+1
static uint8_t cpu_NOP(uint8_t op)
{	
	cpu->REGS[cpu->P]++;

    return 3;
}

/*********/
//if Q==0 then R(P) <- R(P) + 2
static uint8_t cpu_LSNQ(uint8_t op)
{	
	if (io_get(Q) == 0)
	{
		cpu->REGS[cpu->P] += 2;
	};

    return 3;
}

/*********/
//if D!=0 then R(P) <- R(P) + 2
static uint8_t cpu_LSNZ(uint8_t op)
{
	if (cpu->D != 0)
	{
		cpu->REGS[cpu->P] += 2;
	};

    return 3;
}

/*********/
//if DF==0 then R(P) <- R(P) + 2
static uint8_t cpu_LSNF(uint8_t op)
{	
	if (cpu->DF == 0)
	{
		cpu->REGS[cpu->P] += 2;
	};

    return 3;
}

/*********/
//R(P) <- R(P) + 2
static uint8_t cpu_LSKP(uint8_t op)
{	
	cpu->REGS[cpu->P] += 2;

    return 3;
}

/*********/
//if Q==0 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1)
//else R(P) <- R(P)+2
static uint8_t cpu_LBNQ(uint8_t op)
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
	};

    return 3;
}

/*********/
//if D!=0 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1);
//else R(P) <- R(P)+2
static uint8_t cpu_LBNZ(uint8_t op)
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
	};

    return 3;
}

/*********/
//if DF==0 then R(P).1 <- M(R(P)); R(P).0 <- M(R(P)+1);
//else R(P) <- R(P)+2
static uint8_t cpu_LBNF(uint8_t op)
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
	};

    return 3;
}

/*********/
//if IE==1 then R(P) <- R(P)+2
static uint8_t cpu_LSIE(uint8_t op)
{
	if (io_get(IE) == 1)
	{
		cpu->REGS[cpu->P] += 2;
	};

    return 3;
}

/*********/
//if Q==1 then R(P) <- R(P)+2
static uint8_t cpu_LSQ(uint8_t op)
{
	if (io_get(Q) == 1)
	{
		cpu->REGS[cpu->P] += 2;
	};

    return 3;
}

/*********/
//if D==0 then R(P) <- R(P)+2
static uint8_t cpu_LSZ(uint8_t op)
{
	
	if (cpu->D == 0)
	{
		cpu->REGS[cpu->P] += 2;
	};

    return 3;
}

/*********/
//if DF==1 then R(P) <- R(P)+2
static uint8_t cpu_LSDF(uint8_t op)
{
	
	if (cpu->DF == 1)
	{
		cpu->REGS[cpu->P] += 2;
	};

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
static uint8_t cpu_LDX(uint8_t op)
{	
	cpu->D = memory_read(cpu->REGS[cpu->X]);

    return 2;
}

/*********/
//D <- M(R(X)) OR D
static uint8_t cpu_OR(uint8_t op)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	cpu->D = byte | cpu->D;

    return 2;
}

/*********/
//D <- M(R(X)) AND D
static uint8_t cpu_AND(uint8_t op)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	cpu->D = byte & cpu->D;

    return 2;
}

/*********/
//D <- M(R(X)) XOR D
static uint8_t cpu_XOR(uint8_t op)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	cpu->D = byte ^ cpu->D;

    return 2;
}

/*********/
//DF,D <- M(R(X)) + D
static uint8_t cpu_ADD(uint8_t op)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	if ((byte + cpu->D) > 0xFF) { cpu->DF = 1; }
	else { cpu->DF = 0; };

	cpu->D = (uint8_t)(byte+ cpu->D);

    return 2;
}

/*********/
//DF,D <- M(R(X)) - D
static uint8_t cpu_SD(uint8_t op)
{
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	if ( cpu->D > byte ) { cpu->DF = 1; }
	else { cpu->DF = 0; };

	cpu->D = (uint8_t)(byte - cpu->D);

    return 2;
}

/*********/
//Shift D right one bit; MSB(D) <- 0; DF <- LSB(D)
static uint8_t cpu_SHR(uint8_t op)
{	
	cpu->DF = cpu->D & 0x01;
	cpu->D = cpu->D >> 1;

    return 2;
}

/*********/
//DF,D <- D-M(R(X))
static uint8_t cpu_SM(uint8_t op)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->X]);

	if (byte > cpu->D) { cpu->DF = 1; }
	else { cpu->DF = 0; };

	cpu->D = (uint8_t)(cpu->D - byte);

    return 2;
}

/*********/
//D <-M(R(P)); R(P) <- R(P)+1
static uint8_t cpu_LDI(uint8_t op)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	cpu->D = byte;

	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//D <- M(R(P)) OR D; R(P) <- R(P)+1
static uint8_t cpu_ORI(uint8_t op)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	cpu->D = byte | cpu->D;

	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//D <- M(R(P)) AND D; R(P) <- R(P)+1
static uint8_t cpu_ANI(uint8_t op)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	cpu->D = byte & cpu->D;

	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//D <- M(R(P)) XOR D; R(P) <- R(P)+1
static uint8_t cpu_XRI(uint8_t op)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	cpu->D = byte ^ cpu->D;

	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//DF,D <- M(R(P)) + D; R(P) <- R(P)+1
static uint8_t cpu_ADI(uint8_t op)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	if ((cpu->D + byte) > 0xFF) { cpu->DF = 1; }
	else { cpu->DF = 0; };

	cpu->D = (uint8_t)(byte + cpu->D);

	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//DF,D <- M(R(P)) - D; R(P) <- R(P)+1
static uint8_t cpu_SDI(uint8_t op)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	if ( cpu->D > byte ) { cpu->DF = 1; }
	else { cpu->DF = 0; };

	cpu->D = (uint8_t)(byte - cpu->D);

	cpu->REGS[cpu->P]++;

    return 2;
}

/*********/
//Shift left one bit; LSB(D) <- 0 ; DF <- MSB(D)
static uint8_t cpu_SHL(uint8_t op)
{	
	cpu->DF = ((uint8_t)cpu->D >> 7) & 0x01;

	cpu->D = (uint8_t)cpu->D << 1;

    return 2;
}

/*********/
//DF,D <- D - M(R(P)); R(P) <- R(P)+1
static uint8_t cpu_SMI(uint8_t op)
{	
	uint8_t byte = memory_read(cpu->REGS[cpu->P]);

	if ( byte > cpu->D ) { cpu->DF = 1; }
	else { cpu->DF = 0; };

	cpu->D = (uint8_t)(cpu->D - byte);

	cpu->REGS[cpu->P]++;

    return 2;
}

/**********/
static uint8_t cpu_execute0(uint8_t op)
{
	if((op & 0x0F)==0)
	{
		return cpu_IDL(op);
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
        case 0x00: return cpu_BR(op);
        case 0x01: return cpu_BQ(op);
        case 0x02: return cpu_BZ(op);
        case 0x03: return cpu_BDF(op);

        case 0x04: return cpu_B1(op);
        case 0x05: return cpu_B2(op);
        case 0x06: return cpu_B3(op);
        case 0x07: return cpu_B4(op);

        case 0x08: return cpu_SKP(op);
        case 0x09: return cpu_BNQ(op);
        case 0x0A: return cpu_BNZ(op);
        case 0x0B: return cpu_BNF(op);

        case 0x0C: return cpu_BN1(op);
        case 0x0D: return cpu_BN2(op);
        case 0x0E: return cpu_BN3(op);
        case 0x0F: return cpu_BN4(op);
	};

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
        case 0x00: return cpu_IRX(op);

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
        case 0x00: return cpu_RET(op);
        case 0x01: return cpu_DIS(op);
        case 0x02: return cpu_LDXA(op);
        case 0x03: return cpu_STXD(op);
        case 0x04: return cpu_ADC(op);
        case 0x05: return cpu_SDB(op);
        case 0x06: return cpu_SHRC(op);
        case 0x07: return cpu_SMB(op);
        case 0x08: return cpu_SAV(op);
        case 0x09: return cpu_MARK(op);
        case 0x0A: return cpu_REQ(op);
        case 0x0B: return cpu_SEQ(op);
        case 0x0C: return cpu_ADCI(op);
        case 0x0D: return cpu_SDBI(op);
        case 0x0E: return cpu_SHLC(op);
        case 0x0F: return cpu_SMBI(op); 
	};

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
        case 0x00: return cpu_LBR(op);
        case 0x01: return cpu_LBQ(op);
        case 0x02: return cpu_LBZ(op);
        case 0x03: return cpu_LBDF(op);
        case 0x04: return cpu_NOP(op);
        case 0x05: return cpu_LSNQ(op);
        case 0x06: return cpu_LSNZ(op);
        case 0x07: return cpu_LSNF(op);
        case 0x08: return cpu_LSKP(op);
        case 0x09: return cpu_LBNQ(op);
        case 0x0A: return cpu_LBNZ(op);
        case 0x0B: return cpu_LBNF(op);
        case 0x0C: return cpu_LSIE(op);
        case 0x0D: return cpu_LSQ(op);
        case 0x0E: return cpu_LSZ(op);
        case 0x0F: return cpu_LSDF(op);
        
	};

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
        case 0x00: return cpu_LDX(op);
        case 0x01: return cpu_OR(op);
        case 0x02: return cpu_AND(op);
        case 0x03: return cpu_XOR(op);
        case 0x04: return cpu_ADD(op);
        case 0x05: return cpu_SD(op);
        case 0x06: return cpu_SHR(op);
        case 0x07: return cpu_SM(op);
        case 0x08: return cpu_LDI(op);
        case 0x09: return cpu_ORI(op);
        case 0x0A: return cpu_ANI(op);
        case 0x0B: return cpu_XRI(op);
        case 0x0C: return cpu_ADI(op);
        case 0x0D: return cpu_SDI(op);
        case 0x0E: return cpu_SHL(op);
        case 0x0F: return cpu_SMI(op);
	};

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
	};

	return 0;
}

/**********/
void cpu_init()
{
    cpu = alloc(1, sizeof(CPU));

    //set R0 as program counter and make it point to first byte of program
    cpu->P = 0x00;
    cpu->REGS[cpu->P] = PROGRAM_BASE;

}

/**********/
void cpu_close()
{
    free(cpu);
}

/**********/
uint8_t cpu_run()
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