#ifndef _OBLI_MAIN_H
#define _OBLI_MAIN_H

#include <stdbool.h>

#define OBLI_VERSION "2.0a"

#define OBLI_CMD_HELP "help"
#define OBLI_CMD_VERS "version"
#define OBLI_CMD_START "start"
#define OBLI_CMD_STARTW "startWeak"
#define OBLI_CMD_STARTS "startStrong"
#define OBLI_CMD_KILL "kill"
#define OBLI_CMD_INSTALL "install"
#define OBLI_CMD_REMOVE "remove"
#define OBLI_CMD_LSMOD "listModules"

void runShowHelp(const char*);
void runStart(bool);

int _quickPrefix();

#endif // _OBLI_MAIN_H