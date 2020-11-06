#include "processor.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

void initialiseMips(Mips *processor, const char *programFolderName)
{
    FILE *program;
    int res;
    int PC = 0;
    unsigned int tempVal;

    processor->text = malloc(programSize * sizeof(int) / sizeof(char));

    processor->PC = 0;
    processor->HI = 0;
    processor->HO = 0;
    for (int i = 0; i < NBREGISTER; i++)
    {
        processor->registres[i] = 0;
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
            processor->text[PC++] = tempVal;
        }
    }
    fclose(program);
    processor->memory = malloc(sizeof(Register)/sizeof(char));
    processor->memory->next = NULL;
    processor->memory->registerID = 0;
    processor->memory->value = 0;

}

void freeProc(Mips processor)
{
    Register *current = processor.memory;
    Register *prec;
    free(processor.text);
    while (current->next !=NULL)
    {
        prec= current;
        current = current->next; 
        free(prec);
    }
    free(current);
    

}
Register* getregister(Mips processor,int registerID)
{
    Register* reg = processor.memory;
    while ((reg =reg->next) != NULL && reg->registerID != registerID);
    return reg;
}

void addRegister(Mips processor,int registerID,int value)
{
    Register* reg = processor.memory;
    Register* new = malloc(sizeof(Register)/sizeof(char));

    while (reg->next != NULL)
    {
        reg = reg->next;
    }
    reg->next = new;
    
    new->registerID = registerID;
    new->next = NULL;
    new->value = value;
}