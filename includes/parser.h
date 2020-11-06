#ifndef __PARSER_H__

#define __PARSER_H__
#include "opcode.h"
enum type_instruction
{
    R,
    I,
    Ju
};
enum mode_instructionR
{
    A,    // instruction with rs rt rd  ex ADD
    B,    // instruction with rd rt sa
    C,    // instruction with rs rt
    D,    // instruction with rd
    other // instruction without mode
};

extern char *opCodeL[];
extern const int opCodehex[];
extern const char delimiters[];

int test();
void parseFolder(char *src, char *dest);
int parseExpressionStr( char *line,int* flagErr);
int getBeginSpace(const char *line);
typedef struct instruction
{
    char *name;
    int type;
    int mode;
    int hexCode;

} Instruction;

int typeAParseHEX(Instruction instr,int *flagErr);
int typeBParseHEX(Instruction instr,int *flagErr);
int typeCParseHEX(Instruction instr,int *flagErr);
int typeDParseHEX(Instruction instr,int *flagErr);
int typeOtherParseHEX(Instruction instr,int *flagErr);
int instToHex(Instruction instruction,int *flagErr);
void initInstruction(Instruction *instruction);

Instruction instrL[NB_INSTRUCTION];

enum instrERR
{
    instrERR_missing,
    instrERR_parsed,
    instrERR_blankOrComment_line,
    instrERR_error_parsing
};

#endif // __PARSER_H__
