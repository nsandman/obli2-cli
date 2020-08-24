#ifndef _OBLI_LIB_H_
#define _OBLI_LIB_H_

// length the prefix takes up in the path
#define OBLI_PREFIX_LENGTH 8

#define OBLI_MODULES_DIR "modules"

const char *_obli_genPrefixPath(int, ...);
int _obli_createFolderIfNotExists(const char *);

int obli_setupPrefixIfNotExists(void);
int obli_installModule(const char*);

#endif /* _OBLI_LIB_H_ */