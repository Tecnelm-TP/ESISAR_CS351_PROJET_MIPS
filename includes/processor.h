#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__
#define NBREGISTER 32

typedef struct memRegister Register;
typedef struct processor Mips;
 struct processor
{
    unsigned int registres [NBREGISTER];
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

void executeInstruction(unsigned int instruction,Mips *processor);
void executeProgramm(int pas,Mips *processor);
#endif // !__PROCESSOR_H__
