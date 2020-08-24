#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>

#include "obli.h"

int obli_setupPrefixIfNotExists() {
    const char *obliDirPath = _obli_genPrefixPath(0);
    return _obli_createFolderIfNotExists(obliDirPath);
}

/* RETURNS: 0 on create, -1 on error, 1 on do nothing */
int _obli_createFolderIfNotExists(const char *path) {
    struct stat obliDirStat;
    if (!(stat(path, &obliDirStat) == 0 && S_ISDIR(obliDirStat.st_mode))) {
        mkdir(path, 0777);
        return 0;
    }

    return 1;
}

const char *_obli_genPrefixPath(int numPieces, ...) {
    va_list argptr;
    va_start(argptr, numPieces);

    unsigned int lenPieces = numPieces + 2;

    char *pieces[lenPieces];
    pieces[0] = getenv("HOME");
    pieces[1] = "/.obli/";

    unsigned int pathAllocLen = strlen(pieces[0]) + strlen(pieces[1]) + 1;    // ~/.obli/
    for (int i = 0; i < numPieces; i++) {
        char *pathPiece = va_arg(argptr, char *);

        unsigned int lenPiece = strlen(pathPiece);
        pieces[i+2] = malloc(lenPiece + 1);
        strcpy(pieces[i+2], pathPiece);
        pathAllocLen += lenPiece;
    }

    va_end(argptr);

    char *obliDirPath = malloc(pathAllocLen);  

    unsigned int pos = 0;
    for (int j = 0; j < lenPieces; j++) {
        pos += sprintf(&obliDirPath[pos], "%s", pieces[j]);

        // free pointer if it wasn't created from a string literal
        if ((j - numPieces) > 0)
            free(pieces[j]);
    }

    return obliDirPath;
}