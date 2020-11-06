#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "parser.h"
#include "processor.h"

int modePas;

int main(int argc, char *argv[])
{
    static int folderIsHEX;
    static int inputSpecified;
    static int outputSpecified;

    char *src;
    char *dest;
    Mips proc;
    int opt;
    static struct option long_options[] =
        {
            {"pas", no_argument, &modePas, 1},
            {"hex", no_argument, &folderIsHEX, 1},
            {0, 0, 0, 0}};

    while ((opt = getopt_long(argc, argv, "o:i:", long_options, NULL)) != -1)
    {
        switch (opt)
        {

        case 'i':
            inputSpecified = 1;

            src = optarg + getBeginSpace(optarg);

            break;
        case 'o':
            outputSpecified = 1;
            dest = optarg + getBeginSpace(optarg);

            break;
        default:
            break;
        }
    }

    if (inputSpecified)
    {
        if (!outputSpecified)
        {

            dest = strdup(src);

            strcpy(strrchr(dest, '.') + 1, "hex");
        }

        if (!folderIsHEX)
        {
            initInstruction(instrL);
            parseFolder(src, dest);
        }

        initialiseMips(&proc, folderIsHEX ? src : dest);


        
        if (!outputSpecified)
        {
            free(dest);
        }
        freeProc(proc);
    }else
    {
        fprintf(stderr,"ERROR NO SOURCE FILE SCPECIFIED\n");
    }
    

    return 0;
}
