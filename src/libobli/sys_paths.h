#ifndef _SYS_PATH_H
#define _SYS_PATH_H

// length the prefix takes up in the path
#define OBLI_PREFIX_LENGTH 9
#define OBLI_PREFIX_BASE ".oblip"

#define OBLI_MODULES_DIR "modules"
#define OBLI_NETWORK_DIR "network"
#define OBLI_PROGRAM_DIR "bin"

int _obli_createFolderIfNotExists(const char *);

char *_obli_genPrefixPath(int, ...);
void _obli_freePrefixPath(char*);

#endif /* _SYS_PATH_H */