#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/dir.h>
#include "parser.h"
#include "opcode.h"
#include "processor.h"

const char delimiters[] = ", #$\n()";
int programSize = 0;

int getBeginSpace(const char *line)

{
    int result = 0;

    while (line[result] == ' ' && line[result] != '\0')
    {
        result++;
    }

    return result;
}

void parseFolder(const char *src, const char *dest)
{
    FILE *srcFile;
    FILE *destFile;
    int resultparse;
    int flag = instrERR_missing;
    char *line = NULL;
    size_t len = 0;
    char *opcode;

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
            resultparse = parseExpressionStr(line, &flag);

            if (flag == instrERR_parsed)
            {
                fprintf(destFile, "%08X\n", resultparse);
                programSize++;
                fprintf(stderr, "%08X\n", resultparse);
            }
            else if (flag == instrERR_blankOrComment_line)
            {
                fprintf(stderr, "No instruction or comment\n");
            }
            else if (flag == instrERR_error_parsing)
            {
                fprintf(stderr, "ERROR PARSING\n");
            }
            else
            {
                opcode = strtok(line, delimiters);
                fprintf(stderr, "%s :ERROR INSTRUCTION NOT IMPLEMENTED\n", opcode);
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
                resultparse = instToHex(instrL[i], flagErr);
            }
        }
    }
    else
    {
        *flagErr = instrERR_blankOrComment_line;
    }
    return resultparse;
}

int instToHex(Instruction instruction, int *flagErr)
{
    int hex;
    switch (instruction.mode)
    {
    case R:
        switch (instruction.type)
        {
        case RA:
            hex = typeRAParseHEX(instruction, flagErr);
            break;
        case RB:
            hex = typeRBParseHEX(instruction, flagErr);
            break;
        case RC:
            hex = typeRCParseHEX(instruction, flagErr);
            break;
        case RMD:
            hex = typeRDParseHEX(instruction, flagErr);
            break;
        case Rother:
            hex = typeROtherParseHEX(instruction, flagErr);
            break;
        default:
            hex = 0;
            break;
        }
        break;
    case I:
        switch (instruction.type)
        {
        case IA:
            hex = typeIAParseHEX(instruction, flagErr);
            break;
        case IB:
            hex = typeIBParseHEX(instruction, flagErr);
            break;
        case IC:
            hex = typeICParseHEX(instruction, flagErr);
            break;
        case ID:
            hex = typeIDParseHEX(instruction, flagErr);
            break;
        default:
            hex = 0;
            break;
        }

        break;
    case Ju:
        hex = typeJTypeParseHEX(instruction, flagErr);
        break;

    default:
        break;
    }

    return hex;
}

int typeRAParseHEX(Instruction instr, int *flagErr)
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
int typeRBParseHEX(Instruction instr, int *flagErr)
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

int typeRCParseHEX(Instruction instr, int *flagErr)
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

int typeRDParseHEX(Instruction instr, int *flagErr)
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
int typeROtherParseHEX(Instruction instr, int *flagErr)
{
    char *rs;

    int rdi = 0;
    int rsi = 0;
    int rti = 0;
    int sai = 0;

    if (!strcmp(instr.name, "NOP"))
    {
    }
    else if (!strcmp(instr.name, "JR"))
    {
        rs = strtok(NULL, delimiters);
        if (rs == NULL)
        {
            *flagErr = instrERR_error_parsing;
        }
        else
        {
            rsi = atoi(rs);
            sai = 0; //replace with hint
        }
    }

    return instr.hexCode + (sai << 6) + (rdi << 11) + (rti << 16) + (rsi << 21);
    ;
}

int typeIAParseHEX(Instruction instr, int *flagErr)
{
    char *rs;
    char *rt;
    char *offset;

    int rti = 0;
    int rsi = 0;
    int offseti = 0;

    rt = strtok(NULL, delimiters);
    rs = strtok(NULL, delimiters);
    offset = strtok(NULL, delimiters);

    if (rs == NULL || rt == NULL || offset == NULL)
    {
        *flagErr = instrERR_error_parsing;
    }
    else
    {
        offseti = atoi(offset);
        rti = atoi(rt);
        rsi = atoi(rs);

        /* code */
    }

    return (instr.hexCode << 26) + (rsi << 21) + (rti << 16) + (offseti);
}
int typeIBParseHEX(Instruction instr, int *flagErr)
{
    char *rs;
    char *offset;

    int rti = 0;
    int rsi = 0;
    int offseti = 0;

    rs = strtok(NULL, delimiters);
    offset = strtok(NULL, delimiters);

    if (rs == NULL || offset == NULL)
    {
        *flagErr = instrERR_error_parsing;
    }
    else
    {
        offseti = atoi(offset);
        rsi = atoi(rs);
        /* code */
    }

    return (instr.hexCode << 26) + (rsi << 21) + (rti << 16) + (offseti);
}
int typeICParseHEX(Instruction instr, int *flagErr)
{
    char *rt;
    char *offset;

    int rti = 0;
    int rsi = 0;
    int offseti = 0;

    rt = strtok(NULL, delimiters);
    offset = strtok(NULL, delimiters);

    if (rt == NULL || offset == NULL)
    {
        *flagErr = instrERR_error_parsing;
    }
    else
    {
        offseti = atoi(offset);
        rti = atoi(rt);
    }

    return (instr.hexCode << 26) + (rsi << 21) + (rti << 16) + (offseti);
}
int typeIDParseHEX(Instruction instr, int *flagErr)
{
    char *rt;
    char *offset;
    char *base;

    int rti = 0;
    int rsi = 0;
    int offseti = 0;

    rt = strtok(NULL, delimiters);
    offset = strtok(NULL, delimiters);
    base = strtok(NULL, delimiters);

    if (base == NULL || rt == NULL || offset == NULL)
    {
        *flagErr = instrERR_error_parsing;
    }
    else
    {
        offseti = atoi(offset);
        rti = atoi(rt);
        rsi = atoi(base);
        /* code */
    }

    return (instr.hexCode << 26) + (rsi << 21) + (rti << 16) + (offseti);
}

int typeJTypeParseHEX(Instruction instr, int *flagErr)
{
    char *instIndex;
    int instIndexI;

    instIndex = strtok(NULL, delimiters);

    if (instIndex == NULL)
    {
        *flagErr = instrERR_error_parsing;
    }
    else
    {
        instIndexI = atoi(instIndex);
    }

    return instIndexI + (instr.hexCode << 26);
}

void initInstruction(Instruction *instruction)
{
    int i;
    int id = 0;
    for (i = 0; i < NBTYPER; i++)
    {
        for (int b = 0; b < typeNbR[i]; b++)
        {
            instruction[id].name = opCodeL[id];
            instruction[id].mode = R;
            instruction[id].type = i;
            instruction[id].hexCode = opCodehex[id];
            id++;
        }
    }
    beginModeI = id;

    for (i = 0; i < NBTYPEI; i++)
    {
        for (int b = 0; b < typeNbI[i]; b++)
        {
            instruction[id].name = opCodeL[id];
            instruction[id].mode = I;
            instruction[id].type = i;
            instruction[id].hexCode = opCodehex[id];
            id++;
        }
    }
    beginModeJ = id;
    for (i = 0; i < NBTYPEJ; i++)
    {
        for (int b = 0; b < typeNbJ[i]; b++)
        {
            instruction[id].name = opCodeL[id];
            instruction[id].mode = Ju;
            instruction[id].type = i;
            instruction[id].hexCode = opCodehex[id];
            id++;
        }
    }
}
/*
int NBINSTRUCTION = 1;
void V2(const char *path,Instruction** instructionL)
{
    FILE* instr ;
   unsigned int mode;
    unsigned int type;
    char* name;
    unsigned int code;
    int res;
    int cnt = 0;
    

    instr = fopen(path,"r");
    if(instr == NULL){
        fprintf(stderr,"ERROR OPEN FOLDER\n");
        exit(EXIT_FAILURE);
    }

    name = malloc(sizeof(char)*MAXCHARINSTR /sizeof(char));
    while (!feof(instr))
    {

        res = fscanf(instr,"%s %X %X %X\n",name,&code,&mode,&type);
        if(res>0)
        {
            NBINSTRUCTION++;
        }
        
    }
    free(name);
    fseek(instr,0,SEEK_SET);
    *instructionL = malloc(sizeof(Instruction)*NBINSTRUCTION/sizeof(char));

    while (!feof(instr))
    {
        name = malloc(sizeof(char)*MAXCHARINSTR /sizeof(char));
        res = fscanf(instr,"%s %X %X %X\n",name,&code,&mode,&type);
        if(res>0)
        {
            (*instructionL)[cnt].hexCode = code;
            (*instructionL)[cnt].name = name;
            (*instructionL)[cnt].type = type;
            (*instructionL)[cnt++].mode = mode;
        }else
        {
            free(name);
        }
        
    }
    



    fclose(instr);

}
void freeV2(Instruction* instr)
{
    for(int i = 0 ; i<NBINSTRUCTION ; i++)
    {
        free((instr)[i].name);
    }
    free(instr);
}*/

char *opCodeL[] = {
    "ADD",
    "AND",
    "SLT",
    "SUB",
    "OR",
    "XOR",
    /// fin TYPE A
    "ROTR",
    "SLL",
    "SRL",
    /// fin TYPEB
    "DIV",
    "MULT",
    ///fin TYPEC
    "MFHI",
    "MFLO",
    ///fin TYPED
    "NOP",
    "JR",
    "SYSCALL",

    ///Fin mode R
    "BNE",
    "ADDI",
    "BEQ",

    ///fin Itype1
    "BGTZ",
    "BLEZ",
    ///fin Itype2
    "LUI",
    ///fin Itype3
    "SW",
    "LW",
    ///fin Itype4

    ///Fin mode I
    "JAL",
    "J"
    ///Fin mode J
};

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
    NOP,
    JR,
    SYSCALL,

    ///Fin mode R
    BNE,
    ADDI,
    BEQ,

    ///fin Itype1
    BGTZ,
    BLEZ,
    ///fin Itype2
    LUI,
    ///fin Itype3
    SW,
    LW,
    ///fin Itype4

    ///Fin mode I
    JAL,
    J
    ///Fin mode J

};

const int typeNbR[] =
    {
        6, 3, 2, 2, 3

};
const int typeNbJ[] =
    {
        2

};
const int typeNbI[] =
    {
        3, 2, 1, 2

};

int beginModeJ;
int beginModeI;