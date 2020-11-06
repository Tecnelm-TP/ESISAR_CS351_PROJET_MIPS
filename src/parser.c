#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "opcode.h"

const char delimiters[] = ", #$\n";

int getBeginSpace(const char *line)

{
    int result = 0;

    while (line[result] == ' ' && line[result] != '\0')
    {
        result++;
    }

    return result;
}

void parseFolder(char *src, char *dest)
{
    FILE *srcFile;
    FILE *destFile;
    int resultparse;
    int flag = instrERR_missing;
    char *line = NULL;
    size_t len = 0;

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
        getline(&line,&len,srcFile);

        if (line)
        {
            resultparse = parseExpressionStr(line, &flag);

            if (flag == instrERR_parsed)
            {
                fprintf(destFile, "%08X\n", resultparse);
                fprintf(stdout, "%08X\n", resultparse);
            }
            else if (flag == instrERR_blankOrComment_line)
            {
                fprintf(stdout, "No instruction or comment\n");
            }
            else if (flag==instrERR_error_parsing)
            {
                fprintf(stderr, "ERROR PARSING\n");
            }
            else
            {
                fprintf(stderr, "ERROR INSTRUCTION NOT IMPLEMENTED\n");
            }

            free(line);
        }
    }

    fclose(srcFile);
    fclose(destFile);
}
int parseExpressionStr(char *line, int *flagErr)
{
    int nbSpacestart = 0;
    char *opcode;
    int resultparse = 0;
    *flagErr = instrERR_missing;

    nbSpacestart = getBeginSpace(line);

    opcode = strtok(line, delimiters);

    if (opcode != NULL && *(line + nbSpacestart) != '\n' && *(line + nbSpacestart) != '#')
    {

        for (int i = 0; i < NB_INSTRUCTION && !*flagErr; i++)
        {
            if (!strcmp(opcode, opCodeL[i]))
            {
                *flagErr = instrERR_parsed;
                resultparse = instToHex(instrL[i],flagErr);
            }
        }
    }
    else
    {
        *flagErr = instrERR_blankOrComment_line;
    }
    return resultparse;
}
int test()
{
    initInstruction(instrL);
    parseFolder("/mnt/c/Users/cleme/Documents/Programation/ESISAR_CS351_PROJET_MIPS/sujet/exemples2019/tests/in1.txt", "/mnt/c/Users/cleme/Documents/Programation/ESISAR_CS351_PROJET_MIPS/sujet/exemples2019/hexified/in.txt");

    return 0;
}

int instToHex(Instruction instruction, int *flagErr)
{
    int hex;
    switch (instruction.type)
    {
    case A:
        hex = typeAParseHEX(instruction, flagErr);
        break;
    case B:
        hex = typeBParseHEX(instruction, flagErr);
        break;
    case C:
        hex = typeCParseHEX(instruction, flagErr);
        break;
    case D:
        hex = typeDParseHEX(instruction, flagErr);
        break;
    case other:
        hex = typeOtherParseHEX(instruction, flagErr);
        break;
    default:
        break;
    }
    return hex;
}

int typeAParseHEX(Instruction instr, int *flagErr)
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

    if (rd == NULL || rs == NULL || rt == NULL)
    {
        *flagErr = instrERR_error_parsing;
    }
    else
    {
        rti = atoi(rt);
        rdi = atoi(rd);
        rsi = atoi(rs);
        /* code */
    }

    return instr.hexCode + (rdi << 11) + (rti << 16) + (rsi << 21);
}
int typeBParseHEX(Instruction instr, int *flagErr)
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

    if (rd == NULL || rt == NULL || sa == NULL)
    {
        *flagErr = instrERR_error_parsing;
    }
    else
    {
        rdi = atoi(rd);
        rti = atoi(rt);
        sai = atoi(sa);
    }
    if (!strcmp(instr.name, "ROTR"))
    {
        rsi = 1;
    }

    return instr.hexCode + (sai << 6) + (rdi << 11) + (rti << 16) + (rsi << 21);
}

int typeCParseHEX(Instruction instr, int *flagErr)
{
    char *rs;
    char *rt;

    int rdi = 0;
    int rsi = 0;
    int rti = 0;
    int sai = 0;

    rs = strtok(NULL, delimiters);
    rt = strtok(NULL, delimiters);
    if (rs == NULL || rt == NULL)
    {
        *flagErr = instrERR_error_parsing;
    }
    else
    {
        rti = atoi(rt);
        rsi = atoi(rs);
    }

    return (rsi << 21) + (rti << 16) + (rdi << 11) + (sai << 6) + instr.hexCode;
}

int typeDParseHEX(Instruction instr, int *flagErr)
{
    char *rd;

    int rdi = 0;
    int rsi = 0;
    int rti = 0;
    int sai = 0;

    rd = strtok(NULL, delimiters);

    if (rd == NULL)
    {
        *flagErr = instrERR_error_parsing;
    }
    else
    {
        rdi = atoi(rd);
    }
    return instr.hexCode + (sai << 6) + (rdi << 11) + (rti << 16) + (rsi << 21);
}
int typeOtherParseHEX(Instruction instr, int *flagErr)
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