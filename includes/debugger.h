#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

#include "processor.h"

void printRegisters(Mips *processor);
void printMemory(Mips *processor);
void printProgramm(Mips *processor);

#endif // !__DEBUGGER_H__
