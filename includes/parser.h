#ifndef __PARSER_H__

#define __PARSER_H__
#include "opcode.h"
#define MAXCHARINSTR 10

// def types d'instructions
enum type_instruction
{
    R, // register
    I, // immediat
    Ju // jump
};
enum mode_instructionR
{
    RA,     // instruction with rs rt rd
    RB,     // instruction with rd rt sa
    RC,     // instruction with rs rt
    RMD,    // instruction with rd
    Rother, // instruction without mode

};
enum mode_instructionI
{
    IA, //instruction with offset rs rt
    IB, // instruction with offset rs
    IC, //instruction with offset rt
    ID  //instruction LW SW
};
enum mode_instructionJ
{
    Jtype
};

extern char *opCodeL[];
extern const int opCodehex[];
extern const char delimiters[];
extern const char* alias[];

// parse l'ensemble des sources d'un dossier
void parseFolder(const char *src, const char *dest);
// parse une instruction assembleur
int parseExpressionStr(char *line, int *flagErr,int PC);
// retourne l'indice du premier caractère autre que espace de la chaîne
int getBeginSpace(const char *line);

// def structure Instruction
typedef struct instruction
{
    char *name;
    int type;
    int mode;
    int hexCode;

} Instruction;

// def structure label (bonus)
typedef struct label
{
    char* name;
    int value;
    struct label* next;
}Label;

extern Label* labelL;

// parsing des différents types d'instructions
int typeRAParseHEX(Instruction instr, int *flagErr);
int typeRBParseHEX(Instruction instr, int *flagErr);
int typeRCParseHEX(Instruction instr, int *flagErr);
int typeRDParseHEX(Instruction instr, int *flagErr);
int typeROtherParseHEX(Instruction instr, int *flagErr);

int typeIAParseHEX(Instruction instr, int *flagErr,int PC);
int typeIBParseHEX(Instruction instr, int *flagErr,int PC);
int typeICParseHEX(Instruction instr, int *flagErr);
int typeIDParseHEX(Instruction instr, int *flagErr);

int typeJTypeParseHEX(Instruction instr, int *flagErr);

// converti une instruction en son équivalent hexadécimal
int instToHex(Instruction instruction, int *flagErr,int PC);
// initialise une instruction
void initInstruction(Instruction *instruction);

// supprime un label
void freelabel(Label* label);
// recherche un label dans la liste chaînée
Label* searchLabel(char* labelname);

// cherche la correspondance avec l'alias des registres d'usage général
int searchalias(char* check);

// teste si la chaine contient un entier en décimal ou hexadécimal
int isinteger(const char* str);
// converti la chaîne en entier
int convertint(const char* str, int* flagerr);

Instruction instrL[NB_INSTRUCTION];

// def type erreur
enum instrERR
{
    instrERR_missing,
    instrERR_parsed,
    instrERR_blankOrComment_line,
    instrERR_error_parsing
};

#endif // __PARSER_H__
