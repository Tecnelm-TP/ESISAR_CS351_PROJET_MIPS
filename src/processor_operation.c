#include <stdio.h>
#include "processor_operation.h"

void add(int rd, int rt, int rs, Mips *processor)
{
    processor->registres[rd] = processor->registres[rt] + processor->registres[rs];
    fprintf(stdout, "{ADD $%d,$%d,$%d }\n", rd, rs, rt);
}
void andf(int rd, int rt, int rs, Mips *processor)
{
    processor->registres[rd] = processor->registres[rt] & processor->registres[rs];
    fprintf(stdout, "{AND $%d,$%d,$%d }\n", rd, rs, rt);
}
void slt(int rd, int rt, int rs, Mips *processor)
{
    processor->registres[rd] = processor->registres[rs] < processor->registres[rt] ? 1 : 0;
    fprintf(stdout, "{SLT $%d,$%d,$%d }\n", rd, rs, rt);
}
void sub(int rd, int rt, int rs, Mips *processor)
{
    processor->registres[rd] = processor->registres[rt] - processor->registres[rs];
    fprintf(stdout, "{SUB $%d,$%d,$%d }\n", rd, rs, rt);
}
void orf(int rd, int rt, int rs, Mips *processor)
{
    processor->registres[rd] = processor->registres[rt] | processor->registres[rs];
    fprintf(stdout, "{OR $%d,$%d,$%d }\n", rd, rs, rt);
}
void xorf(int rd, int rt, int rs, Mips *processor)
{
    processor->registres[rd] = processor->registres[rt] ^ processor->registres[rs];
    fprintf(stdout, "{XOR $%d,$%d,$%d }\n", rd, rs, rt);
}

void rotr(int rd, int rt, int sa, Mips *processor)
{
}
void sll(int rd, int rt, int sa, Mips *processor)
{
    processor->registres[rd] = processor->registres[rt] << sa;
    fprintf(stdout, "{SLL $%d,$%d,%d }\n", rd, rt, sa);
}
void srl(int rd, int rt, int sa, Mips *processor)
{
    processor->registres[rd] = processor->registres[rt] >> sa;
    fprintf(stdout, "{SRL $%d,$%d,%d }\n", rd, rt, sa);
}

void divf(int rt, int rs, Mips *processor)
{
    processor->HI = processor->registres[rs] % processor->registres[rt];
    processor->LO = processor->registres[rs] / processor->registres[rt];
    fprintf(stdout, "{DIV $%d,$%d }\n", rs, rt);
}
void mult(int rt, int rs, Mips *processor)
{
    long result = processor->registres[rs] * processor->registres[rt];
    processor->HI = (result >> 32) & 0xFFFFFFFF;
    processor->LO = result & 0xFFFFFFFF;
    fprintf(stdout, "{MULT $%d,$%d}\n", rs, rt);
}
void mfhi(int rd, Mips *processor)
{
    processor->registres[rd] = processor->HI;
    fprintf(stdout, "{MFHI $%d }\n", rd);
}
void mflo(int rd, Mips *processor)
{
    processor->registres[rd] = processor->LO;
    fprintf(stdout, "{MFLO $%d }\n", rd);
}

void jr(int rs, Mips *processor)
{
    processor->PC = processor->registres[rs];

    fprintf(stdout, "{JR $%d }\n", rs);
}
void syscall(int code, Mips *processor)
{
}

void bne(int rs, int rt, short int offset, Mips *processor)
{
}
void addi(int rs, int rt, short int offset, Mips *processor)
{
    processor->registres[rt] = processor->registres[rs] + offset;
    fprintf(stdout, "{ADDI $%d,$%d,0x%X }\n", rt, rs, offset);
}
void beq(int rs, int rt, short int offset, Mips *processor)
{
}
void bgtz(int rs, short int offset, Mips *processor)
{
}
void blez(int rs, short int offset, Mips *processor)
{
}
void lui(int rt, short int offset, Mips *processor)
{
    processor->registres[rt] = offset << 16;
    fprintf(stdout, "{LUI %d,0x%X}\n", rt, offset);
}

void sw(int rt, short int offset, int base, Mips *processor)
{
    Register *reg = getregister(processor, processor->registres[base] + offset);
    if (reg == NULL)
    {
        addRegister(processor, processor->registres[base] + offset, processor->registres[rt]);
    }
    else
    {
        reg->value = processor->registres[rt];
    }
    fprintf(stdout, "{SW $%d,0x%X(0x%X)}\n", rt, offset, base);
}
void lw(int rt, short int offset, int base, Mips *processor)
{
    Register *reg = getregister(processor, processor->registres[base] + offset);
    if (reg == NULL)
    {
        addRegister(processor, processor->registres[base] + offset, 0);
        processor->registres[rt] = 0;
    }
    else
    {
        processor->registres[rt] = reg->value;
    }
    fprintf(stdout, "{LW $%d,0x%X(0x%X)}\n", rt, offset, base);
}

void jal(int offset, Mips *processor)
{
}
void j(int offset, Mips *processor)
{
}