#ifndef __PARSER_H__

#define __PARSER_H__
#include "opcode.h"
int convertOpcode(char *opcode);
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
void parseExpression(char *src, char *dest);
typedef struct instruction
{
    char *name;
    int type;
    int mode;
    int hexCode;

} Instruction;

int typeAParseHEX(Instruction instr);
int typeBParseHEX(Instruction instr);
int typeCParseHEX(Instruction instr);
int typeDParseHEX(Instruction instr);
int typeOtherParseHEX(Instruction instr);
int instToHex(Instruction instruction);
void initInstruction(Instruction *instruction);

Instruction instrL[NB_INSTRUCTION];

#endif // __PARSER_H__
