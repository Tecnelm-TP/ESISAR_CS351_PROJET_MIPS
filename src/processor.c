#include "processor.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

void initialiseMips(Mips proc, const char *programFolderName)
{
    FILE *program;
    int res;
    int PC = 0;
    int tempVal;

    proc.text = malloc(programSize * sizeof(int) / sizeof(char));
    proc.PC = 0;
    proc.HI = 0;
    proc.HO = 0;
    for (int i = 0; i < NBREGISTER; i++)
    {
        proc.registres[i] = 0;
    }
    program = fopen(programFolderName, "r");
    if (program == NULL)
    {
        fprintf(stderr, "ERROR OPEN PROGRAMM FOLDER\n");
        exit(1);
    }
    while (!feof(program))
    {
        res = fscanf(program, "%X\n", &tempVal);
        if (res == 1)
        {
            fprintf(stdout,"%08X\t%08X\n",PC<<2,tempVal);
            proc.text[PC++] = tempVal;
        }
    }
    fclose(program);
    proc.memory = malloc(sizeof(Register)/sizeof(char));
    proc.memory->next = NULL;
    proc.memory->registerID = 0;
    proc.memory->value = 0;

}

void freeProc(Mips proc)
{
    Register *current = proc.memory;
    Register *prec;
    free(proc.text);
    
    do
    {
        prec = current;
        current = current->next;
        free(prec);
    } while (current->next != NULL);
    
    
}