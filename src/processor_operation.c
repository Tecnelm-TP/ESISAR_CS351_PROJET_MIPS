#include <stdio.h>
#include "processor_operation.h"

void add(int rd, int rt, int rs, Mips *processor)
{
    processor->registres[rd] = processor->registres[rt] + processor->registres[rs];
    processor->PC += 4;
    fprintf(stdout, "{ADD $%d,$%d,$%d }\n", rd, rs, rt);
}
void andf(int rd, int rt, int rs, Mips *processor)
{
    processor->registres[rd] = processor->registres[rt] & processor->registres[rs];
    processor->PC += 4;
    fprintf(stdout, "{AND $%d,$%d,$%d }\n", rd, rs, rt);
}
void slt(int rd, int rt, int rs, Mips *processor)
{
    processor->registres[rd] = processor->registres[rs] < processor->registres[rt] ? 1 : 0;
    processor->PC += 4;
    fprintf(stdout, "{SLT $%d,$%d,$%d }\n", rd, rs, rt);
}
void sub(int rd, int rt, int rs, Mips *processor)
{
    processor->registres[rd] = processor->registres[rs] - processor->registres[rt];
    processor->PC += 4;
    fprintf(stdout, "{SUB $%d,$%d,$%d }\n", rd, rs, rt);
}
void orf(int rd, int rt, int rs, Mips *processor)
{
    processor->registres[rd] = processor->registres[rt] | processor->registres[rs];
    processor->PC += 4;
    fprintf(stdout, "{OR $%d,$%d,$%d }\n", rd, rs, rt);
}
void xorf(int rd, int rt, int rs, Mips *processor)
{
    processor->registres[rd] = processor->registres[rt] ^ processor->registres[rs];
    processor->PC += 4;
    fprintf(stdout, "{XOR $%d,$%d,$%d }\n", rd, rs, rt);
}

void rotr(int rd, int rt, int sa, Mips *processor)
{

    processor->registres[rd] = (processor->registres[rt] << sa) + (processor->registres[rt] >> (32 - sa));
    processor->PC += 4;

    fprintf(stdout, "{ROTR $%d,$%d,0X%0X }\n", rd, rt, sa);
}
void sll(int rd, int rt, int sa, Mips *processor)
{
    processor->registres[rd] = processor->registres[rt] << sa;
    processor->PC += 4;
    fprintf(stdout, "{SLL $%d,$%d,%d }\n", rd, rt, sa);
}
void srl(int rd, int rt, int sa, Mips *processor)
{
    processor->registres[rd] = processor->registres[rt] >> sa;
    processor->PC += 4;

    fprintf(stdout, "{SRL $%d,$%d,%d }\n", rd, rt, sa);
}

void divf(int rt, int rs, Mips *processor)
{
    processor->HI = processor->registres[rs] % processor->registres[rt];
    processor->LO = processor->registres[rs] / processor->registres[rt];
    processor->PC += 4;

    fprintf(stdout, "{DIV $%d,$%d }\n", rs, rt);
}
void mult(int rt, int rs, Mips *processor)
{
    long result = processor->registres[rs] * processor->registres[rt];
    processor->HI = (result >> 32) & 0xFFFFFFFF;
    processor->LO = result & 0xFFFFFFFF;
    processor->PC += 4;

    fprintf(stdout, "{MULT $%d,$%d}\n", rs, rt);
}
void mfhi(int rd, Mips *processor)
{
    processor->registres[rd] = processor->HI;
    processor->PC += 4;

    fprintf(stdout, "{MFHI $%d }\n", rd);
}
void mflo(int rd, Mips *processor)
{
    processor->registres[rd] = processor->LO;
    processor->PC += 4;

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
    if (processor->registres[rs] != processor->registres[rt])
    {
        processor->PC += offset << 2;
    }
    else
    {
        processor->PC += 4;
    }

    fprintf(stdout, "{BNE $%d,$%d,0x%X }\n", rs, rt, offset);
}
void addi(int rs, int rt, short int offset, Mips *processor)
{
    processor->registres[rt] = processor->registres[rs] + offset;
    processor->PC += 4;

    fprintf(stdout, "{ADDI $%d,$%d,0x%X }\n", rt, rs, offset);
}
void beq(int rs, int rt, short int offset, Mips *processor)
{
    if (processor->registres[rs] == processor->registres[rt])
    {
        processor->PC += offset << 2;
    }
    else
    {
        processor->PC += 4;
    }

    fprintf(stdout, "{BEQ $%d,$%d,0x%X }\n", rs, rt, offset);
}
void bgtz(int rs, short int offset, Mips *processor)
{
    if (processor->registres[rs] > 0)
    {
        processor->PC += offset << 2;
    }
    else
    {
        processor->PC += 4;
    }

    fprintf(stdout, "{BGTZ $%d,0x%X }\n", rs, offset);
}
void blez(int rs, short int offset, Mips *processor)
{
    if (processor->registres[rs] <= 0)
    {
        processor->PC += offset << 2;
    }
    else
    {
        processor->PC += 4;
    }

    fprintf(stdout, "{BLEZ $%d,0x%X }\n", rs, offset);
}
void lui(int rt, short int offset, Mips *processor)
{
    processor->registres[rt] = offset << 16;
    processor->PC += 4;

    fprintf(stdout, "{LUI %d,0x%X}\n", rt, offset);
}

void sw(int rt, short int offset, int base, Mips *processor)
{
    Register *reg = getregister(processor, processor->registres[base] + (offset>>2));
    if (reg == NULL)
    {
        addRegister(processor, processor->registres[base] +( offset), processor->registres[rt]);
    }
    else
    {
        reg->value = processor->registres[rt];
    }
    processor->PC += 4;

    fprintf(stdout, "{SW $%d,0x%X($%d)}\n", rt, offset, base);
}
void lw(int rt, short int offset, int base, Mips *processor)
{
    Register *reg = getregister(processor, processor->registres[base] + (offset));
    if (reg == NULL)
    {
        addRegister(processor, processor->registres[base] + (offset), 0);
        processor->registres[rt] = 0;
    }
    else
    {
        processor->registres[rt] = reg->value;
    }
    processor->PC += 4;

    fprintf(stdout, "{LW $%d,0x%X($%d)}\n", rt, offset, base);
}

void jal(int offset, Mips *processor)
{
    processor->registres[31] = processor->PC + 4;
    processor->PC = (processor->PC & 0xf0000000) + (offset);
    fprintf(stdout, "{JAL 0x%X }\n", offset);
}
void j(int offset, Mips *processor)
{
    processor->PC = (processor->PC & 0xf0000000) + (offset << 2);
    fprintf(stdout, "{J 0x%X }\n", offset);
}
void nop(Mips *processor)
{
    fprintf(stdout, "{NOP}\n");
    processor->PC+=4;
}