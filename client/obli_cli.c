#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <colorprinter.h>

#include "obli.h"
#include "obli_cli.h"
#include "constants.h"

const char *commands[] = {
    OBLI_CMD_HELP,
    OBLI_CMD_VERS,
    OBLI_CMD_START,
    OBLI_CMD_STARTW,
    OBLI_CMD_STARTS,
    OBLI_CMD_KILL,
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
    "Kill daemon and running obli instances",
    "Install module",
    "Remove module",
    "List all installed modules"
};

void runShowHelp(const char *programName) {
    printf("Usage: %s [option]. Please run \"%s help\" for help.\n\n", programName, programName);

    uint8_t numCommands = LEN_ARRAY(commands);
    for (int i = 0; i < numCommands; i++) {
        printf("\t\033[1m%s\033[0m: %s\n", commands[i], commandBlurbs[i]);
    }
}

int _quickPrefix() {
    int prefixCreated = obli_setupPrefixIfNotExists();
    switch (prefixCreated) {
        case OBLI_FILESYS_ERROR:
            cfputs("Error creating prefix.", red, stderr);
            break;

        case OBLI_PREFIX_CREATED:
            cputs("Created prefix.", green);
            break;

        case OBLI_PREFIX_POPULATED:
            cputs("Populated prefix.", green);
            break;

        case OBLI_PREFIX_OK:
        default:
            break; 
    }

    // returns Unix error codes but negative
    if (prefixCreated < 0)
        exit(prefixCreated * -1);
    
    return prefixCreated;
}

// TODO: start daemon
void runStart(bool strong) {
    _quickPrefix();
    obli_launchDaemon();
}

void runEnd() {
    _quickPrefix();
    obli_killDaemon();
}

void runInstall(int num, char **names) {
    _quickPrefix();

    for (int i = 0; i < num; i++) {
        const char *module = names[i];
        unsigned int hash = obli_hashModuleName(module);

        printf("%d. Install: %x (%s)... ", i+1, hash, module);
        obli_installModule(module);

        cputs("Done!", green);
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
    else if (strcmp(commandName, OBLI_CMD_KILL) == 0) {
        runEnd();
    }
    else if (strcmp(commandName, OBLI_CMD_INSTALL) == 0) {
        if (argc < 3) {
            cfputs("Please specify at least one module (format: <github/repository> or <url to git repo>) to install\n", red, stderr);
            return EPERM;
        }
        runInstall(argc-2, argv+2);
    }
    else {
        cfprintf(stderr, red, "Unknown command <%s>.\n", commandName);
        runShowHelp(argv[0]);
    }

    return 0;
}