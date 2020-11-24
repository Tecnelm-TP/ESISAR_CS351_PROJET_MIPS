#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__
#define NBREGISTER 32

// def structure processeur MIPS
typedef struct processor Mips;
struct processor
{
    unsigned int registres[NBREGISTER];
    int PC;
    int HI;
    int LO;
    int programSize;
    unsigned int *text;
    Register *memory;
};

// def élément mémoire
typedef struct memRegister Register;
struct memRegister
{
    int registerID;
    int value;
    struct memRegister *next;
};

// initialisation structure
void initialiseMips(Mips *processor, const char *programFolderName);
// libération de l'espace mémoire
void freeProc(Mips *processor);

// récupération d'un élément mémoire
Register *getregister(Mips *processor, int registerID);
// ajout d'un élément mémoire
void addRegister(Mips *processor, int registerID, int value);

// exécution du programme / instructions en fonction des différents modes
void executeInstruction(unsigned int instruction, Mips *processor);
void executeProgramm(int pas, Mips *processor);
void executeInteractiv(Mips *processor);
void step(Mips *processor);

// initialisation processeur
void initialiseProcessor(Mips *processor);

#endif // !__PROCESSOR_H__