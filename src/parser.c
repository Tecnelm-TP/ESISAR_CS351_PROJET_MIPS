#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "opcode.h"

const char delimiters[] = ", #$";

int convertOpcodeTohex(char *opcode)
{
    int result = 0;
    for (int i = 0; i < NB_INSTRUCTION; i++)
    {
        if (!strcmp(opcode, opCodeL[i]))
        {
            result = opCodehex[i];
        }
    }
    return result;
}
int getBeginSpace(const char* line)

{
    int result =0 ;

    while (line[result]==' ' && line[result]!='\0'   )
    {
        result++;
    }

    return result;
    
}
void parseExpression(char *src, char *dest)
{
    FILE *srcFile;
    FILE *destFile;
    int resultparse;
    char *opcode;
    int flag = 0;
    char *line = NULL;
    int len = 0;
    int nbSpacestart=0;

    srcFile = fopen(src, "r");
    if (srcFile == NULL)
    {
        fprintf(stderr, "ERROR OPEN FOLDER SRC\n");
        exit(1);
    }

    destFile = fopen(dest, "w");
    if (destFile == NULL)
    {
        fprintf(stderr, "ERROR OPEN FOLDER DEST\n");
        exit(1);
    }

    while (!feof(srcFile))
    {
        line = NULL;
        getline(&line, &len, srcFile);
        if (line)
        {
            
            nbSpacestart = getBeginSpace(line);

            opcode = strtok(line, delimiters);

            if (opcode != NULL && *(line+nbSpacestart)!='\n' && *(line+nbSpacestart) != '#')
            {

                for (int i = 0; i < NB_INSTRUCTION && !flag; i++)
                {
                    if (!strcmp(opcode, opCodeL[i]))
                    {
                        resultparse = instToHex(instrL[i]);
                        flag = 1;
                    }
                }
                if (flag)
                {
                    flag = 0;
                    fprintf(destFile, "%08X\n", resultparse);
                    fprintf(stdout, "%08X\n", resultparse);
                }
                else
                {
                    fprintf(stderr, "ERROR INSTRUCTION NOT IMPLEMENTED\n");
                }
            }

            free(line);
        }
    }

    fclose(srcFile);
    fclose(destFile);
}
int test()
{
    /*
    char *opcode;
    int result;
    int flag = 0;
    char src[] = "ADD $2,$3,$4 #salutttt comment pas pris en compte";
    initInstruction(instrL);

    Instruction instruction = {"ADD", R, A, ADD};
    opcode = strtok(src, delimiters);

    for (int i = 0; i < NB_INSTRUCTION && !flag; i++)
    {
        if (!strcmp(opcode, opCodeL[i]))
        {
            result = instToHex(instrL[i]);
            flag = 1;
        }
    }

    fprintf(stdout, "%08X\n", result);*/

    initInstruction(instrL);
    parseExpression("/mnt/c/Users/cleme/Documents/Programation/ESISAR_CS351_PROJET_MIPS/sujet/exemples2019/tests/in1.txt", "/mnt/c/Users/cleme/Documents/Programation/ESISAR_CS351_PROJET_MIPS/sujet/exemples2019/hexified/in.txt");

    return 0;
}

int instToHex(Instruction instruction)
{
    int hex;
    switch (instruction.type)
    {
    case A:
        hex = typeAParseHEX(instruction);
        break;
    case B:
        hex = typeBParseHEX(instruction);
        break;
    case C:
        hex = typeCParseHEX(instruction);
        break;
    case D:
        hex = typeDParseHEX(instruction);
        break;
    case other:
        hex = typeOtherParseHEX(instruction);
        break;
    default:
        break;
    }
    return hex;
}

int typeAParseHEX(Instruction instr)
{
    char *rs;
    char *rt;
    char *rd;
    int rdi = 0;
    int rsi = 0;
    int rti = 0;

    rd = strtok(NULL, delimiters);
    rs = strtok(NULL, delimiters);
    rt = strtok(NULL, delimiters);

    rdi = atoi(rd);
    rsi = atoi(rs);
    rti = atoi(rt);

    return instr.hexCode + (rdi << 11) + (rti << 16) + (rsi << 21); // 00641020
}
int typeBParseHEX(Instruction instr)
{
    char *rt;
    char *rd;
    char *sa;

    int rdi = 0;
    int rsi = 0;
    int rti = 0;
    int sai = 0;

    rd = strtok(NULL, delimiters);
    rt = strtok(NULL, delimiters);
    sa = strtok(NULL, delimiters);

    rdi = atoi(rd);
    rti = atoi(rt);
    sai = atoi(sa);

    if (!strcmp(instr.name, "ROTR"))
    {
        rsi = 1;
    }

    return instr.hexCode + (sai << 6) + (rdi << 11) + (rti << 16) + (rsi << 21);
}

int typeCParseHEX(Instruction instr)
{
    char *rs;
    char *rt;

    int rdi = 0;
    int rsi = 0;
    int rti = 0;
    int sai = 0;

    rs = strtok(NULL, delimiters);
    rt = strtok(NULL, delimiters);

    rti = atoi(rt);
    rsi = atoi(rs);

    return (rsi << 21) + (rti << 16) + (rdi << 11) + (sai << 6) + instr.hexCode;
}

int typeDParseHEX(Instruction instr)
{
    char *rd;

    int rdi = 0;
    int rsi = 0;
    int rti = 0;
    int sai = 0;

    rd = strtok(NULL, delimiters);

    rdi = atoi(rd);

    return instr.hexCode + (sai << 6) + (rdi << 11) + (rti << 16) + (rsi << 21);
}
int typeOtherParseHEX(Instruction instr)
{
    int hex;
    if (!strcmp(instr.name, "NOP"))
    {
        hex = 0;
    }

    return hex;
}

void initInstruction(Instruction *instruction)
{
    int i;
    int id = 0;
    for (i = 0; i < NBTYPE; i++)
    {
        for (int b = 0; b < typeNb[i]; b++)
        {
            instruction[id].name = opCodeL[id];
            instruction[id].mode = R;
            instruction[id].type = i;
            instruction[id].hexCode = opCodehex[id];
            id++;
        }
    }
}

char *opCodeL[] = {
    "ADD",
    "AND",
    "SLT",
    "SUB",
    "OR",
    "XOR",

    "ROTR",
    "SLL",
    "SRL",

    "DIV",
    "MULT",

    "MFHI",
    "MFLO",

    "NOP"};

const int opCodehex[] = {
    ADD,
    AND,
    SLT,
    SUB,
    OR,
    XOR,
    /// fin TYPE A
    ROTR,
    SLL,
    SRL,
    /// fin TYPEB
    DIV,
    MULT,
    ///fin TYPEC
    MFHI,
    MFLO,
    ///fin TYPED
    NOP

};

const int typeNb[] =
    {
        6, 3, 2, 2, 1

};