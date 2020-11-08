#include <stdio.h>
#include "debugger.h"

void printRegisters(Mips *processor)
{
    fprintf(stdout, "-----------------------------------------------\n");

    for (int i = 0; i < NBREGISTER; i++)
    {
        fprintf(stdout, "$r%d\t=\t%d\n", i, processor->registres[i]);
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "HI\t=\t%d\t", processor->HI);
    fprintf(stdout, "$LO\t=\t%d\t", processor->LO);
    fprintf(stdout, "$PC\t=\t%d\n", processor->PC);

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

        fprintf(stdout, "REGISTER_ID: %08X\tVALUE: %08X\n", current->registerID, current->value);
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
        fprintf(stdout, "%08X\t%08X\n", i<<2, processor->text[i]);
    }
    fprintf(stdout, "-----------------------------------------------\n");
}