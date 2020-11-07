#define _GNU_SOURCE


#define getSA(i) sa = (i & SA) >> 5
#define getRD(i) rd = (i & RD) >> 11
#define getRT(i) rt = (i & RT) >> 16
#define getRS(i) rs = (i & RS) >> 21

#define flush(std, c) while (c != '\n' && c != EOF) c = getc(stdin);

#define wait() int c = getc(stdin); flush(sdin, c);
#include <stdio.h>
#include <stdlib.h>
#include "processor.h"
#include "processor_operation.h"
#include "parser.h"
#include "string.h"

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
            fprintf(stdout, "%08X\t%08X\n", PC << 2, tempVal);
            processor->text[PC++] = tempVal;
        }
    }
    fclose(program);
    processor->memory = malloc(sizeof(Register) / sizeof(char));
    processor->memory->next = NULL;
    processor->memory->registerID = 0;
    processor->memory->value = 0;
}

void freeProc(Mips *processor)
{
    Register *current = processor->memory;
    Register *prec;
    free(processor->text);
    while (current->next != NULL)
    {
        prec = current;
        current = current->next;
        free(prec);
    }
    free(current);
}
Register *getregister(Mips *processor, int registerID)
{
    Register *reg = processor->memory;
    while ((reg = reg->next) != NULL && reg->registerID != registerID)
        ;
    return reg;
}

void addRegister(Mips *processor, int registerID, int value)
{
    Register *reg = processor->memory;
    Register *new = malloc(sizeof(Register) / sizeof(char));

    while (reg->next != NULL)
    {
        reg = reg->next;
    }
    reg->next = new;

    new->registerID = registerID;
    new->next = NULL;
    new->value = value;
}
void executeProgramm(int pas, Mips *processor)
{
    while (processor->PC != (programSize << 2))
    {
        executeInstruction((processor->text)[(processor->PC) >> 2], processor);
        if (pas)
        {
            wait();
        }
    }
}
void executeInteractiv(Mips *processor)
{
    char* line ;
    size_t len;
    int flagErr;
    int instruction;
    int flagStop = 0;
    fprintf(stdout,"Entering in interactiv mode : EXIT to stop \n");

    do
    {
        line = NULL;
        fprintf(stdout,"enter instuction : \n");
        getline(&line,&len,stdin);

        if(!strncmp(line,"EXIT",4))
            flagStop = 1;
        else
        {
            instruction = parseExpressionStr(line,&flagErr);
            executeInstruction(instruction,processor);
        }
        free(line);

        
    } while (!flagStop);
    
    
    
    

}
void executeInstruction(unsigned int instruction, Mips *processor)
{
    instruction = 0;
    int rs = 0, rd = 0, rt = 0, sa = 0, offset = 0;

    if (instruction != 0)
    {
        if (!(instruction & IINTRCODE))
        {
            getRS(instruction);
            getRD(instruction);
            getRT(instruction);
            switch (instruction & RINTRCODE)
            {
            case ADD:
                add(rd, rt, rs, processor);
                break;
            case AND:
                andf(rd, rt, rs, processor);
                break;
            case SLT:
                slt(rd, rt, rs, processor);
                break;
            case SUB:
                sub(rd, rt, rs, processor);
                break;
            case OR:
                orf(rd, rt, rs, processor);
                break;
            case XOR:
                xorf(rd, rt, rs, processor);
                break;

            case ROTR:
                if (rs)
                    rotr(rd, rt, sa, processor);
                else
                    srl(rs, rt, rs, processor);
                break;
            case SLL:
                sll(rd, rt, sa, processor);
                break;

            case DIV:
                divf(rt, rs, processor);

                break;
            case MULT:
                mult(rt, rs, processor);
                break;

            case MFHI:
                mfhi(rd, processor);
                break;
            case MFLO:
                mflo(rd, processor);
                break;

            case JR:
                jr(rs, processor);
                break;
            case SYSCALL:
                syscall(offset, processor);
                break;

            default:
                break;
            }
        }
        else
        {
            switch (instruction & IINTRCODE)
            {
            case BNE:
                bne(rs, rt, offset, processor);
                break;
            case ADDI:
                addi(rs, rt, offset, processor);
                break;
            case BEQ:
                beq(rs, rt, offset, processor);
                break;
            case BGTZ:
                bgtz(rs, offset, processor);
                break;
            case BLEZ:
                blez(rs, offset, processor);
                break;
            case LUI:
                lui(rs, offset, processor);
                break;
            case SW:
                sw(rt, offset, rs, processor);
                break;
            case LW:
                lw(rt, offset, rs, processor);
                break;
            case JAL:
                jal(offset, processor);
                break;
            case J:
                j(offset, processor);
                break;
            default:
                break;
            }
        }
    }
}