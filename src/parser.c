#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

int convertOpcodeTohex(char* opcode)
{
    int result  = 0 ;
    for (int i = 0 ; i < NB_INSTRUCTION ; i++)
    {
        if (!strcmp(opcode,opCodeL[i]))
        {
            result = opCodehex[i];
        }
    }
    return result;
}

int test()
{
    char* instruction = "ADD rt, rs, rd  ";
    char* opcode;
    char* rs ;
    char* rt ;
    char* rd ;


    opcode = strtok(instruction, " ");
    rd = strtok(NULL, ",");
    rt = strtok(NULL, ",");
    rs = strtok(NULL, "#");

    printf("%s\n",opcode);
    printf("%s\n",rd);
    printf("%s\n",rt);
    printf("%s\n",rs);

return 0;

}



const char *opCodeL[]= {
    "ADD",
    ""
};
const char opCodehex[]={
    ""
};