#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/dir.h>
#include <ctype.h>
#include "parser.h"
#include "opcode.h"
#include "processor.h"

const char delimiters[] = ", #\n()\r";
Label *labelL;
int permissiveMode = 0;

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
    char *parseLine = NULL;
    size_t len = 0;
    char *opcode;
    labelL = malloc(sizeof(Label) / sizeof(char));
    labelL->name = malloc(sizeof(char));
    labelL->name = strcpy(labelL->name, "");
    labelL->next = NULL;
    labelL->value = 0;
    int position = 0;

    Label *last = labelL;
    Label *lastfill = labelL;

    char *temp1;
    char *temp2;
    char *temp3;

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
        fclose(srcFile);
        exit(1);
    }
    permissiveMode = 1;
    //-----------------------------------------------------------------------------//
    while (!feof(srcFile))
    {
        line = NULL;

        getline(&line, &len, srcFile);

        temp1 = strchr(line, ':');
        temp2 = strchr(line, '#');

        if (((temp1 && temp2 && (temp1 < temp2)) || (temp1)))
        {
            last->next = malloc(sizeof(Label) / sizeof(char));
            last = last->next;
            last->value = 0;
            last->next = NULL;
            temp3 = strtok(line, ":");
            last->name = malloc(sizeof(char) * (strlen(temp3) + 1));
            strcpy(last->name, temp3);
        }

        if (line)
        {
            if (((temp1 && temp2 && (temp1 < temp2)) || (temp1)))
            {
                parseLine = strtok(NULL, "");
            }
            else
            {
                parseLine = line;
            }

            resultparse = parseExpressionStr(parseLine, &flag, position);

            if (flag == instrERR_parsed)
            {
                while (lastfill->next != NULL)
                {
                    lastfill = lastfill->next;
                    lastfill->value = position;
                }
                position++;
            }
            free(line);
        }
    }
    //-----------------------------------------------------------------//
    permissiveMode = 0;
    fseek(srcFile, 0, SEEK_SET);
    position = 0;

    while (!feof(srcFile))
    {
        line = NULL;

        getline(&line, &len, srcFile);

        temp1 = strchr(line, ':');
        temp2 = strchr(line, '#');

        if (line)
        {
            if (((temp1 && temp2 && (temp1 < temp2)) || (temp1)))
            {
                strtok(line, ":");
                parseLine = strtok(NULL, "");
            }
            else
            {
                parseLine = line;
            }
            resultparse = parseExpressionStr(parseLine, &flag, position);

            if (flag == instrERR_parsed)
            {
                fprintf(destFile, "%08X\n", resultparse);
                fprintf(stderr, "%08X\n", resultparse);
                position++;
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

    freelabel(labelL);
    fclose(srcFile);
    fclose(destFile);
}
int parseExpressionStr(char *line, int *flagErr, int PC)
{
    int nbSpacestart = 0;
    char *opcode;
    int resultparse = 0;
    *flagErr = instrERR_missing;
    char *temp;

    nbSpacestart = getBeginSpace(line);

    opcode = strtok(line, delimiters);

    if (opcode != NULL && *(line + nbSpacestart) != '\n' && *(line + nbSpacestart) != '#')
    {
        temp = opcode;
        while (*temp)
        {
            *temp = toupper((unsigned char)*temp);
            temp++;
        }

        for (int i = 0; i < NB_INSTRUCTION && !*flagErr; i++)
        {
            if (!strcmp(opcode, opCodeL[i]))
            {
                *flagErr = instrERR_parsed;
                resultparse = instToHex(instrL[i], flagErr, PC);
            }
        }
    }
    else
    {
        *flagErr = instrERR_blankOrComment_line;
    }
    return resultparse;
}

int instToHex(Instruction instruction, int *flagErr, int PC)
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
            hex = typeIAParseHEX(instruction, flagErr, PC);
            break;
        case IB:
            hex = typeIBParseHEX(instruction, flagErr, PC);
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
        if (*rt == '$' && *rd == '$' && *rs == '$')
        {

            rsi = searchalias(rs + 1);
            if (rsi == -1)
                rsi = convertint(rs + 1, flagErr);

            rti = searchalias(rt + 1);
            if (rti == -1)
                rti = convertint(rt + 1, flagErr);

            rdi = searchalias(rd + 1);
            if (rdi == -1)
                rdi = convertint(rd + 1, flagErr);

            if (rsi < 0 || rti < 0 || rdi < 0 || rsi > 31 || rti > 31 || rdi > 31)
                *flagErr = instrERR_error_parsing;
        }
        else
        {
            *flagErr = instrERR_error_parsing;
        }

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
        if (*rt == '$' && *rd == '$')
        {
            rti = searchalias(rt + 1);
            if (rti == -1)
                rti = convertint(rt + 1, flagErr);

            rdi = searchalias(rd + 1);
            if (rdi == -1)
                rdi = convertint(rd + 1, flagErr);
            if (rti < 0 || rdi < 0 || rti > 31 || rdi > 31)
                *flagErr = instrERR_error_parsing;
        }
        else
        {
            *flagErr = instrERR_error_parsing;
        }
        sai = convertint(sa, flagErr) & 0x1F;
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

        if (*rt == '$' && *rs == '$')
        {
            rsi = searchalias(rs + 1);
            if (rsi == -1)
                rsi = convertint(rs + 1, flagErr);

            rti = searchalias(rt + 1);
            if (rti == -1)
                rti = convertint(rt + 1, flagErr);

            if (rsi < 0 || rti < 0 || rsi > 31 || rti > 31)
                *flagErr = instrERR_error_parsing;
        }
        else
        {
            *flagErr = instrERR_error_parsing;
        }
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
        if (*rd == '$')
        {
            rdi = searchalias(rd + 1);
            if (rdi == -1)
                rdi = convertint(rd + 1, flagErr);

            if (rdi < 0 || rdi > 31)
                *flagErr = instrERR_error_parsing;
        }
        else
        {
            *flagErr = instrERR_error_parsing;
        }
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
            if (*rs == '$')
            {
                rsi = searchalias(rs + 1);
                if (rsi == -1)
                    rsi = convertint(rs + 1, flagErr);

                if (rsi < 0 || rsi > 31)
                    *flagErr = instrERR_error_parsing;
            }
            else
            {
                *flagErr = instrERR_error_parsing;
            }
            sai = 0; //replace with hint
        }
    }

    return instr.hexCode + (sai << 6) + (rdi << 11) + (rti << 16) + (rsi << 21);
    
}

int typeIAParseHEX(Instruction instr, int *flagErr, int PC)
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
    Label *label;

    if (rs == NULL || rt == NULL || offset == NULL)
    {
        *flagErr = instrERR_error_parsing;
    }
    else
    {
        label = searchLabel(offset);
        if (label == NULL)
        {
            if (!permissiveMode)
                offseti = convertint(offset, flagErr) & 0xFFFF;
        }
        else
        {
            offseti = (label->value - PC) & 0xFFFF;
        }
        if (*rt == '$' && *rs == '$')
        {
            rsi = searchalias(rs + 1);
            if (rsi == -1)
                rsi = convertint(rs + 1, flagErr);

            rti = searchalias(rt + 1);
            if (rti == -1)
                rti = convertint(rt + 1, flagErr);

            if (rsi < 0 || rti < 0 || rsi > 31 || rti > 31)
                *flagErr = instrERR_error_parsing;
        }
        else
        {
            *flagErr = instrERR_error_parsing;
        }

        /* code */
    }

    return (instr.hexCode << 26) + (rsi << 21) + (rti << 16) + (offseti);
}
int typeIBParseHEX(Instruction instr, int *flagErr, int PC)
{
    char *rs;
    char *offset;

    int rti = 0;
    int rsi = 0;
    int offseti = 0;
    Label *label;

    rs = strtok(NULL, delimiters);
    offset = strtok(NULL, delimiters);

    if (rs == NULL || offset == NULL)
    {
        *flagErr = instrERR_error_parsing;
    }
    else
    {
        label = searchLabel(offset);
        if (label == NULL)
        {
            if (!permissiveMode)

                offseti = convertint(offset, flagErr) & 0xFFFF;
        }
        else
        {
            offseti = (label->value - PC) & 0xFFFF;
        }
        if (*rs == '$')
        {
            rsi = searchalias(rs + 1);
            if (rsi == -1)
            {
                rsi = convertint(rs + 1, flagErr);
            }
            if (rsi < 0 || rsi > 31)
                *flagErr = instrERR_error_parsing;
        }
        else
        {
            *flagErr = instrERR_error_parsing;
        }
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
        offseti = convertint(offset, flagErr);
        if (*rt == '$')
        {
            rti = searchalias(rt + 1);
            if (rti == -1)
            {
                rti = convertint(rt + 1, flagErr);
            }
            if (rti < 0 || rti > 31)
                *flagErr = instrERR_error_parsing;
        }
        else
        {
            *flagErr = instrERR_error_parsing;
        }
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
    if (*offset == '$')
    {
        base = offset;
    }

    if (base == NULL || rt == NULL || offset == NULL)
    {
        *flagErr = instrERR_error_parsing;
    }
    else
    {
        if (*offset == '$')
        {
            offseti = 0;
        }
        else
        {
            /* code */
            offseti = convertint(offset, flagErr);
        }

        if (*base == '$')
        {
            rsi = convertint(base + 1, flagErr);
        }
        else
        {
            *flagErr = instrERR_error_parsing;
        }

        if (*rt == '$')
        {

            rti = searchalias(rt + 1);
            if (rti == -1)
            {
                rti = convertint(rt + 1, flagErr);
            }

            if (rti < 0 || rti > 31)
                *flagErr = instrERR_error_parsing;
        }
        else
        {
            *flagErr = instrERR_error_parsing;
        }
    }

    return (instr.hexCode << 26) + (rsi << 21) + (rti << 16) + (offseti);
}

int typeJTypeParseHEX(Instruction instr, int *flagErr)
{
    char *instIndex;
    int instIndexI = 0;
    Label *label;
    instIndex = strtok(NULL, delimiters);

    if (instIndex == NULL)
    {
        *flagErr = instrERR_error_parsing;
    }
    else
    {
        if (instIndex != NULL)
        {
            label = searchLabel(instIndex);

            if (label == NULL)
            {
                if (!permissiveMode)

                    instIndexI = convertint(instIndex, flagErr);
            }
            else
            {
                instIndexI = label->value;
            }
            instIndexI &= 0x01FFFFFF;
        }
    }

    return instIndexI + (instr.hexCode << 26);
}

Label *searchLabel(char *labelname)
{
    Label *label = labelL;
    int flag = 1;

    while (flag && label != NULL)
    {
        flag = strcmp(labelname, label->name);
        if (flag)
            label = label->next;
    }

    return label;
}
int convertint(const char *str, int *flagerr)
{
    int res = 0;
    int len = strlen(str);

    if (isinteger(str))
    {
        if (len > 2)
        {
            if (str[0] == '0' && str[1] == 'x')
            {
                res = strtol(str, NULL, 16) & 0xFFFF;
            }
            else
            {
                res = atoi(str) & 0xFFFF;
                /* code */
            }
        }
        else
            res = atoi(str);
    }
    else
    {
        *flagerr = instrERR_error_parsing;
    }
    return res;
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
int searchalias(char *check)
{
    int ret = -1;

    for (int i = 0; i < 32 && ret == -1; i++)
    {
        if (!strcmp(check, alias[i]))
            ret = i;
    }
    return ret;
}

int isinteger(const char *str)
{
    int ret = -1;
    int len = strlen(str);
    if (len > 2)
    {
        if (str[0] == '0' && str[1] == 'x')
        {
            ret = 1;
            for (int i = 2; i < len && ret == 1; i++)
            {
                if (!((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'A' && str[i] <= 'F')))
                    ret = 0;
            }
        }
    }
    if (ret == -1)
    {
        ret = 1;
        for (int i = 0; i < len && ret == 1; i++)
        {
            if (!(((str[i] >= '0' && str[i] <= '9') || (i == 0 && str[i] == '-'))))
                ret = 0;
        }
    }
    return ret == -1 ? 0 : ret;
}
void freelabel(Label *label)
{

    Label *current = label;
    Label *prec;
    while (current->next != NULL)
    {
        prec = current;
        current = current->next;
        free(prec->name);
        free(prec);
    }
    if (current != NULL)
    {
        free(current->name);
        free(current);
    }
}

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

const char *alias[] =
    {
        "zero",
        "at",
        "v0",
        "v1",
        "a0",
        "a1",
        "a2",
        "a3",
        "t0",
        "t1",
        "t2",
        "t3",
        "t4",
        "t5",
        "t6",
        "t7",
        "s0",
        "s1",
        "s2",
        "s3",
        "s4",
        "s5",
        "s6",
        "s7",
        "t8",
        "t9",
        "k0",
        "k1",
        "gp",
        "sp",
        "fp",
        "ra"};
