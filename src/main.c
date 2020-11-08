#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "parser.h"
#include "processor.h"

int main(int argc, char *argv[])
{
    static int folderIsHEX;
    static int inputSpecified;
    static int outputSpecified;
    static int needHelp;
    static int modePas;

    char *src;
    char *dest;
    Mips proc;
    int opt;
    static struct option long_options[] =
        {
            {"pas", no_argument, &modePas, 1},
            {"hex", no_argument, &folderIsHEX, 1},
            {"help", no_argument, &needHelp, 1},
            {0, 0, 0, 0}};

    while ((opt = getopt_long(argc, argv, "o:i:hp", long_options, NULL)) != -1)
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
        case 'h':
            needHelp = 1;
            break;
        case 'p':
            modePas = 1;
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
        executeProgramm(modePas, &proc);

        if (!outputSpecified)
        {
            free(dest);
        }
        freeProc(&proc);
    }
    else
    {
        if (needHelp)
        {
            fprintf(stdout, " -i: specified input file\n -o: specified output file\n --hex: folder input is already parsed in hexcode\n --pas/-p: activate mode step by step\n");
        }
        else
        {
            executeInteractiv(&proc);
        }
        }

    return 0;
}
