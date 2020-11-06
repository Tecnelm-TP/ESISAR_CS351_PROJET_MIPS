#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__
#define NBREGISTER 32

typedef struct memRegister Register;
typedef struct processor Mips;
extern int modePas;
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

void initialiseMips(Mips *processor,const char *programFolderName);
void freeProc(Mips processor);

Register* getregister(Mips processor,int registerID);
void addRegister(Mips processor,int registerID,int value);

#endif // !__PROCESSOR_H__
