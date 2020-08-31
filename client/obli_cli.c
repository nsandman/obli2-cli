#include <obli.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <colorprinter.h>

#include "run.h"
#include "constants.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        runShowHelp(argv[0]);
        return EPERM;
    }

    const char *commandName = argv[1];
    if (strcmp(commandName, OBLI_CMD_HELP) == 0) {
        runShowHelp(argv[0]);
    }
    else if (strcmp(commandName, OBLI_CMD_VERS) == 0) {
        printf("Obli %s\n", OBLI_VERSION);
    }
    else if (strcmp(commandName, OBLI_CMD_START) == 0) {
        runStart(false);
    }
    else if (strcmp(commandName, OBLI_CMD_KILL) == 0) {
        runEnd();
    }
    else if (strcmp(commandName, OBLI_CMD_INSTALL) == 0) {
        if (argc < 3) {
            cfputs("Please specify at least one module (format: <github/repository> or <url to git repo>) to install\n", RED, stderr);
            return EPERM;
        }
        runInstall(argc-2, argv+2);
    }
    else {
        cfprintf(stderr, RED, "Unknown command <%s>.\n", commandName);
        runShowHelp(argv[0]);
    }

    return 0;
}