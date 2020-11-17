#ifndef __PARSER_H__

#define __PARSER_H__
#include "opcode.h"
#define MAXCHARINSTR 10
enum type_instruction
{
    R,
    I,
    Ju
};
enum mode_instructionR
{
    RA,     // instruction with rs rt rd  ex ADD
    RB,     // instruction with rd rt sa
    RC,     // instruction with rs rt
    RMD,    // instruction with rd
    Rother, // instruction without mode

};
enum mode_instructionI
{
    IA, //instruction with offset rs rt
    IB, // instruction with offset rs
    IC, //instruction with offset rt
    ID  //instruction LW SW
};
enum mode_instructionJ
{
    Jtype
};

extern char *opCodeL[];
extern const int opCodehex[];
extern const char delimiters[];
extern const char* alias[];

void parseFolder(const char *src, const char *dest);
int parseExpressionStr(char *line, int *flagErr,int PC);
int getBeginSpace(const char *line);
typedef struct instruction
{
    char *name;
    int type;
    int mode;
    int hexCode;

} Instruction;

typedef struct label
{
    char* name;
    int value;
    struct label* next;
}Label;

extern Label* labelL;

int typeRAParseHEX(Instruction instr, int *flagErr);
int typeRBParseHEX(Instruction instr, int *flagErr);
int typeRCParseHEX(Instruction instr, int *flagErr);
int typeRDParseHEX(Instruction instr, int *flagErr);
int typeROtherParseHEX(Instruction instr, int *flagErr);

int typeIAParseHEX(Instruction instr, int *flagErr,int PC);
int typeIBParseHEX(Instruction instr, int *flagErr,int PC);
int typeICParseHEX(Instruction instr, int *flagErr);
int typeIDParseHEX(Instruction instr, int *flagErr);

int typeJTypeParseHEX(Instruction instr, int *flagErr);

int instToHex(Instruction instruction, int *flagErr,int PC);
void initInstruction(Instruction *instruction);
void freelabel(Label* label);
Label* searchLabel(char* labelname);
int searchalias(char* check);

int isinteger(const char* str);

/*
extern int NBINSTRUCTION;

void V2(const char *path,Instruction** instructionL);
void freeV2(Instruction* instr);*/

Instruction instrL[NB_INSTRUCTION];

enum instrERR
{
    instrERR_missing,
    instrERR_parsed,
    instrERR_blankOrComment_line,
    instrERR_error_parsing
};

#endif // __PARSER_H__
