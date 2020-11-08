#define _GNU_SOURCE

#define getSA(i, sa) sa = (i & SA) >> 6
#define getRD(i, rd) rd = (i & RD) >> 11
#define getRT(i, rt) rt = (i & RT) >> 16
#define getRS(i, rs) rs = (i & RS) >> 21
#define getOffset(i, offset) offset = (i & OFFSET)
#define getCode(i, offset) offset = (i & CODE) >> 6
#define getinstr_index(i, offset) offset = (i & INSTRINDEX)

#define flush(std, c)             \
    while (c != '\n' && c != EOF) \
        c = getc(stdin);

#include <stdio.h>
#include <stdlib.h>
#include "processor.h"
#include "processor_operation.h"
#include "parser.h"
#include "string.h"
#include "debugger.h"

void initialiseMips(Mips *processor, const char *programFolderName)
{
    FILE *program;
    int res;
    int PC = 0;
    unsigned int tempVal;
    initialiseProcessor(processor);

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
    while (processor->PC != (processor->programSize << 2))
    {
        executeInstruction((processor->text)[(processor->PC) >> 2], processor);
        processor->PC += 4;

        if (pas)
        {
            step(processor);
        }
    }
}
void executeInteractiv(Mips *processor)
{
    char *line;
    size_t len;
    int flagErr;
    int instruction;
    int flagStop = 0;
    initialiseProcessor(processor);
    initInstruction(instrL);

    fprintf(stdout, "Entering in interactiv mode : EXIT to stop \n");

    do
    {
        line = NULL;
        fprintf(stdout, "enter instuction : \n");
        getline(&line, &len, stdin);

        if (!strncmp(line, "EXIT", 4))
            flagStop = 1;
        else
        {
            instruction = parseExpressionStr(line, &flagErr);
            switch (flagErr)
            {
            case instrERR_parsed:
                fprintf(stderr, "%08X\n", instruction);
                executeInstruction(instruction, processor);
                break;
            case instrERR_missing:

                fprintf(stderr, "%s :ERROR INSTRUCTION NOT IMPLEMENTED\n", strtok(line, delimiters));
                break;
            case instrERR_blankOrComment_line:
                fprintf(stderr, "No instruction or comment\n");
                break;
            case instrERR_error_parsing:
                fprintf(stderr, "ERROR PARSING\n");
                break;

            default:
                break;
            }
            step(processor);
        }
        free(line);
    } while (!flagStop);
}
void executeInstruction(unsigned int instruction, Mips *processor)
{
    int rs = 0, rd = 0, rt = 0, sa = 0, offset = 0;

    if (instruction != 0)
    {
        if (!(instruction & IINTRCODE))
        {
            getRS(instruction, rs);
            getRD(instruction, rd);
            getRT(instruction, rt);
            getCode(instruction, offset);
            switch (instruction & INTRCODE)
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
                getSA(instruction, sa);

                if (rs)
                    rotr(rd, rt, sa, processor);
                else
                    srl(rd, rt, sa, processor);
                break;
            case SLL:
                getSA(instruction, sa);
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
                getCode(instruction, offset);
                syscall(offset, processor);
                break;

            default:
                break;
            }
        }
        else
        {
            getRS(instruction, rs);
            getRT(instruction, rt);
            getOffset(instruction, offset);
            switch ((instruction >> 26) & INTRCODE)
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
                getinstr_index(instruction, offset);
                jal(offset, processor);
                break;
            case J:
                getinstr_index(instruction, offset);
                j(offset, processor);
                break;
            default:
                break;
            }
        }
    }
    else
    {
        fprintf(stdout, "{NOP }\n");
    }

    processor->registres[0] = 0;
}

void step(Mips *processor)
{

    int c, last;

    do
    {
        fprintf(stdout, "press\nEnter: go next instruction\nm: print memory\np: print program\nr: print register\n");
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

    } while (c != '\n' && c != EOF);
}

void initialiseProcessor(Mips *processor)
{

    processor->programSize = programSize;
    processor->text = malloc(processor->programSize * sizeof(int) / sizeof(char));

    processor->PC = 0;
    processor->HI = 0;
    processor->LO = 0;
    for (int i = 0; i < NBREGISTER; i++)
    {
        processor->registres[i] = 0;
    }

    processor->memory = malloc(sizeof(Register) / sizeof(char));
    processor->memory->next = NULL;
    processor->memory->registerID = 0;
    processor->memory->value = 0;
}