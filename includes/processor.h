#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__
#define NBREGISTER 32

typedef struct memRegister Register;
typedef struct processor Mips;
 struct processor
{
    int registres [NBREGISTER];
    int PC;

    int HI;
    int HO;

    int *text;
    Register* memory;
};

extern int programSize;

 struct memRegister
{
    int registerID;
    int value;
    struct memRegister* next;
};

void initialiseMips(Mips proc,const char *programFolderName);
void freeProc(Mips proc);



#endif // !__PROCESSOR_H__
