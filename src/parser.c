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

    char* opcode;
    char* rs ;
    char* rt ;
    char* rd ;
    char src[] = "ADD $2,$3,$4 #salutttt comment pas pris en compte";
    const char delimiters[] = ", #$";
    Instruction instruction = {"ADD",R,A,0b100000};
    
    opcode = strtok(src, delimiters);
   


    printf ("%s\n",opcode);
    //printf ("%s\n",rd);
    //printf ("%s\n",rs);
    //printf ("%s\n",rt);

    int inst = typeAParseHEX(instruction);


fprintf(stdout,"%08X\n",inst);

int a =3;
    

return 0;

}
int typeAParseHEX(Instruction instr)
{
    const char delimiters[] = ", #$";

    char* rs ;
    char* rt ;
    char* rd ;
    rd = strtok(NULL,delimiters);
    rs = strtok(NULL,delimiters);
    rt = strtok(NULL,delimiters);

    int rdi = atoi(rd);
    int rsi = atoi(rs);
    int rti = atoi(rt);

    return instr.hexCode+(rdi<<11)+(rti <<16)+(rsi<<21); // 00641020
}



const char *opCodeL[]= {
    "ADD",
    ""
};
const char opCodehex[]={
    ""
};