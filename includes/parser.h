#ifndef __PARSER_H__

#define __PARSER_H__

#define NB_INSTRUCTION 3

int convertOpcode(char* opcode);
enum type_instruction
{
    R,I,J
};
enum mode_instruction
{
    A
};

extern const char *opCodeL[];
extern const char opCodehex[];
int test();
typedef struct instruction
{
    char* name ;
    int type;
    int mode;
    int hexCode;
    
}Instruction;

int typeAParseHEX(Instruction instr);





#endif // __PARSER_H__
