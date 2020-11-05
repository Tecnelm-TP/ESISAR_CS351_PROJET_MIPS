#ifndef __PARSER_H__

#define __PARSER_H__

#define NB_INSTRUCTION 3

int convertOpcode(char* opcode);
enum opCodehex
{
    ADD
};

extern const char *opCodeL[];
extern const char opCodehex[];
int test();

#endif // __PARSER_H__
