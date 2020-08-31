#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>

#include "obli.h"
#include "constants.h"
#include "sys_paths.h"

/* RETURNS: 0 on create, -1 on error, 1 on do nothing */
int _obli_createFolderIfNotExists(const char *path) {
    struct stat obliDirStat;
    if (!(stat(path, &obliDirStat) == 0 && S_ISDIR(obliDirStat.st_mode))) {
        mkdir(path, 0777);
        return 0;
    }

    return 1;
}

/** USAGE: numPieces num of va_args will be joined
 *  to obli prefix with slashes **/
char *_obli_genPrefixPath(int numPieces, ...) {
    va_list argptr;
    va_start(argptr, numPieces);

    // TODO: Allow people to define prefix base through environment variable
    unsigned int lenPieces = numPieces + 2;

    char *pieces[lenPieces];
    pieces[0] = getenv("HOME");
    pieces[1] = OBLI_PREFIX_BASE;

    unsigned int pathAllocLen = strlen(pieces[0]) + strlen(pieces[1]) + 1;    // ~/.obli/
    for (int i = 0; i < numPieces; i++) {
        char *pathPiece = va_arg(argptr, char *);

        unsigned int lenPiece = strlen(pathPiece) + 1;      // we also add a /
        pieces[i+2] = malloc(lenPiece + 1);
        strcpy(pieces[i+2], pathPiece);
        pathAllocLen += lenPiece;
    }

    va_end(argptr);

    char *obliDirPath = malloc(pathAllocLen);  

    unsigned int pos = 0;
    for (int j = 0; j < lenPieces; j++) {
        char *fmt = "/%s";
        if (j == 0)
            fmt = "%s";
            
        pos += sprintf(&obliDirPath[pos], fmt, pieces[j]);

        // free pointer if it wasn't created from a string literal
        // don't write this as a constant so we don't accidentally break anything later
        if (j >= (lenPieces - numPieces))
            free(pieces[j]);
    }

    return obliDirPath;
}

void _obli_freePrefixPath(char *prefixPath) {
    free(prefixPath);
}

void _obli_populatePrefix(int *creationStatus) {
    const char *subdirectories[] = {
        OBLI_DIR_MODULES,
        OBLI_DIR_PROGRAM,
        OBLI_DIR_NETWORK,
        OBLI_DIR_TMP,
        OBLI_DIR_LOG
    };
    for (int i = 0; i < LEN_ARRAY(subdirectories); i++) {
        char *newFolderPath = _obli_genPrefixPath(1, subdirectories[i]);

        int lastFolderResult = _obli_createFolderIfNotExists(newFolderPath);
        if (lastFolderResult < 0) {
            continue;           // move on if error
        } else if ((lastFolderResult == 0) && (*creationStatus != OBLI_PREFIX_CREATED)) {
            *creationStatus = OBLI_PREFIX_POPULATED;
        }

        _obli_freePrefixPath(newFolderPath);
    }
}