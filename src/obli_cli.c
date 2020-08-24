#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "obli_cli.h"

#include "obli.h"

const char *commands[] = {
    OBLI_CMD_HELP,
    OBLI_CMD_VERS,
    OBLI_CMD_START,
    OBLI_CMD_STARTW,
    OBLI_CMD_STARTS,
    OBLI_CMD_INSTALL,
    OBLI_CMD_REMOVE,
    OBLI_CMD_LSMOD    
};

const char *commandBlurbs[] = {
    "Shows this help message",
    "Print current version string",
    "Alias for startWeak",
    "Start in weak mode with limited automation access",
    "Start in strong mode with full access to automation features",
    "Install module",
    "Remove module",
    "List all installed modules"
};

void runShowHelp(const char *programName) {
    printf("Usage: %s [option]. Please run \"%s help\" for help.\n\n", programName, programName);

    uint8_t numCommands = ARRAY_SIZE(commands);
    for (int i = 0; i < numCommands; i++) {
        printf("\t\033[1m%s\033[0m: %s\n", commands[i], commandBlurbs[i]);
    }
}

void runStart(bool strong) {
    int prefixCreated = obli_setupPrefixIfNotExists();
    if (prefixCreated == 0) {
        puts("Created prefix in ~/.obli");
    } else if (prefixCreated == -1) {
        puts("Error creating prefix in ~/.obli");
    }
}

void runInstall(int num, char **names) {
    for (int i = 0; i < num; i++) {
        const char *module = names[i];

        printf("%d. Installing module %s... ", i+1, module);
        obli_installModule(module);

        puts("Done!");
    }
}

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
    else if (strcmp(commandName, OBLI_CMD_STARTW) == 0) {
        runStart(false);
    }
    else if (strcmp(commandName, OBLI_CMD_STARTS) == 0) {
        runStart(true);
    }
    else if (strcmp(commandName, OBLI_CMD_INSTALL) == 0) {
        if (argc < 3) {
            puts("Please specify at least one module (format: <github/repository> or git url) to install");
        }
        runInstall(argc-2, argv+2);
    }
    else {
        printf("Unknown command <%s>.\n", commandName);
        runShowHelp(argv[0]);
    }

    return 0;
}