#include "parser.h"
#include "processor.h"
int main(int argc, char const *argv[])
{
    Mips proc;

    test();
    initialiseMips(proc,"/mnt/c/Users/cleme/Documents/Programation/ESISAR_CS351_PROJET_MIPS/sujet/exemples2019/hexified/in.txt");
    freeProc(proc);
    
    return 0;
}
