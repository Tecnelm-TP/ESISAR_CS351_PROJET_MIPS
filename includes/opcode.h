

#ifndef __OPCODE__
#define __OPCODE__

#define NB_INSTRUCTION 26
#define ADD 0x20
#define AND 0x24
#define SLT 0x2A
#define SUB 0x22
#define OR 0x25
#define XOR 0x26
#define MFLO 0x12
#define NOP 0x0
#define DIV 0x1A
#define MFHI 0x10
#define MULT 0x18
#define SLL 0x0
#define SRL 0x2
#define JAL 0x3
#define J 0x2
#define ROTR 0x2
#define JR 0x04
#define SYSCALL 0xC
#define BGTZ 0x7
#define BNE 0x5
#define LW 0x23
#define ADDI 0x8
#define BEQ 0x4
#define BLEZ 0x6
#define LUI 0xF
#define SW 0x2B

#define NBTYPER 5
#define NBTYPEJ 1
#define NBTYPEI 4

extern const int typeNbR[NBTYPER];
extern const int typeNbJ[NBTYPEJ];
extern const int typeNbI[NBTYPEI];

extern int beginModeJ;
extern int beginModeI;

#define NBTYPEOTHER

#endif // !__OPCODE__
