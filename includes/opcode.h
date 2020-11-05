

#ifndef __OPCODE__
#define __OPCODE__

#define NB_INSTRUCTION 14
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

#define NBTYPE 5

extern const int typeNb[5];

#define NBTYPEOTHER

#endif // !__OPCODE__
