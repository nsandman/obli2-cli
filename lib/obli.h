#ifndef _OBLI_LIB_H_
#define _OBLI_LIB_H_

#include <stdbool.h>

#define OBLI_FILESYS_ERROR -1
#define OBLI_PREFIX_CREATED 0
#define OBLI_PREFIX_OK 1
#define OBLI_PREFIX_POPULATED 2

// public API
int obli_setupPrefixIfNotExists(void);
unsigned int obli_hashModuleName(const char*);
int obli_installModule(const char*);
int obli_launchDaemon(void);
void obli_killDaemon(void);
bool obli_isDaemonRunning(void);

// private
bool _obli_isDaemonRunning(const char*);

#endif /* _OBLI_LIB_H_ */