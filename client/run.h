#ifndef _RUN_H_
#define _RUN_H_

#include "obli_cli.h"

int _quickPrefix();

void runShowHelp(const char*);
void runStart(bool);
void runEnd(void);
void runInstall(int, char**);

#endif /* _RUN_H_ */