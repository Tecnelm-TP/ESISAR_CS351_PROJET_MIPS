
#define flush(std, c)             \
    while (c != '\n' && c != EOF) \
        c = getc(stdin);


#include <stdio.h>
#include "debugger.h"



void printRegisters(Mips *processor)
{
    fprintf(stdout, "-----------------------------------------------\n");

    for (int i = 0; i < NBREGISTER; i+=4)
    {
        fprintf(stdout, "$r%d = %d \t", i, processor->registres[i]);
        fprintf(stdout, "$r%d = %d \t", i+1, processor->registres[i+1]);
        fprintf(stdout, "$r%d = %d \t", i+2, processor->registres[i+2]);
        fprintf(stdout, "$r%d = %d\n", i+3, processor->registres[i+3]);
    }
    fprintf(stdout, "\n\n");
    fprintf(stdout, "HI = %d \t", processor->HI);
    fprintf(stdout, "$LO = %d \t", processor->LO);
    fprintf(stdout, "$PC = %d \n", processor->PC);

    fprintf(stdout, "-----------------------------------------------\n");
}
void printMemory(Mips *processor)
{
    Register *current = processor->memory;
    fprintf(stdout, "-----------------------------------------------\n");

    fprintf(stdout, "PRINT MEMORY\n");
    while (current->next != NULL)
    {
        current = current->next;

        fprintf(stdout, "REGISTER_ID: 0x%08X\tVALUE: 0x%08X\n", current->registerID, current->value);
    }
    fprintf(stdout, "-----------------------------------------------\n");
}
void printProgramm(Mips *processor)
{
    fprintf(stdout, "-----------------------------------------------\n");

    fprintf(stdout, "PC\t\tHEXCODE\n");
    for (int i = 0; i < processor->programSize; i++)
    {
        //fprintf(stdout, " %08H\t %08H \n", i, processor->text[i]);
        fprintf(stdout, "%08d\t%08X", i << 2, processor->text[i]);
        if(processor->PC == i<<2)
        {
            fprintf(stdout," <--");
        }
        fprintf(stdout,"\n");

    }
    fprintf(stdout, "-----------------------------------------------\n");
}

void step(Mips *processor)
{

    int c, last;

    fprintf(stdout, "-----------------------------------------------\n");

    do
    {

        fprintf(stdout, "Enter: go next instruction\nm: print memory\np: print program\nr: print register\n");
        c = getc(stdin);
        last = c;
        switch (c)
        {
        case 'm':
            printMemory(processor);
            break;
        case 'r':
            printRegisters(processor);
            break;
        case 'p':
            printProgramm(processor);
            break;
        case '\n':
            break;
        default:
            break;
        }
        flush(stdin, last);
        fprintf(stdout, "-----------------------------------------------\n");

    } while (c != '\n' && c != EOF);

}
