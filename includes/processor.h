#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__
#define NBREGISTER 32
typedef struct processor
{
    int registres [NBREGISTER];
    int PC;

    int HI;
    int HO;

    int *text;

}Mips;

extern int programSize;

void initialiseMips(Mips proc,const char *programFolderName);
void freeProc(Mips proc);



#endif // !__PROCESSOR_H__
