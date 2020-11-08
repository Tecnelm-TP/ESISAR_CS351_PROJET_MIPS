#ifndef __PROCESSOR_OPERATION_H__
#define __PROCESSOR_OPERATION_H__
#include "processor.h"

void add(int rd, int rt, int rs, Mips *processor);
void andf(int rd, int rt, int rs, Mips *processor);
void slt(int rd, int rt, int rs, Mips *processor);
void sub(int rd, int rt, int rs, Mips *processor);
void orf(int rd, int rt, int rs, Mips *processor);
void xorf(int rd, int rt, int rs, Mips *processor);

void rotr(int rd, int rt, int sa, Mips *processor);
void sll(int rd, int rt, int sa, Mips *processor);
void srl(int rd, int rt, int sa, Mips *processor);

void divf(int rt, int rs, Mips *processor);
void mult(int rt, int rs, Mips *processor);

void mfhi(int rd, Mips *processor);
void mflo(int rd, Mips *processor);

void jr(int rs, Mips *processor);
void syscall(int code, Mips *processor);

void bne(int rs, int rt, short int offset, Mips *processor);
void addi(int rs, int rt, short int offset, Mips *processor);
void beq(int rs, int rt, short int offset, Mips *processor);
void bgtz(int rs, short int offset, Mips *processor);
void blez(int rs, short int offset, Mips *processor);
void lui(int rt, short int offset, Mips *processor);

void sw(int rt, short int offset, int base, Mips *processor);
void lw(int rt, short int offset, int base, Mips *processor);

void jal(int offset, Mips *processor);
void j(int offset, Mips *processor);

#endif
