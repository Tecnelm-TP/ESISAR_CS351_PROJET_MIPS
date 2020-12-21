#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

#include "processor.h"
/**
 * affiche tous les registre du processeur passé en paramètre
 * */
void printRegisters(Mips *processor);

/**
 * affiche les registres qui ont été utilisé dans le programmes 
 * */
void printMemory(Mips *processor);

/**
 * Affiche le programe ainsi que la prosition du PC dedans
 * */
void printProgramm(Mips *processor);

/**
 * permet d'éxécuter un pas, demander quoi faire pour le debug 
 * */
void step(Mips *processor);


#endif // !__DEBUGGER_H__
