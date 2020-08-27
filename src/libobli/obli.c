#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <git2.h>
#include <git2/clone.h>

#include <murmurhash.h>

#include "obli.h"
#include "constants.h"
#include "sys_paths.h"

int obli_setupPrefixIfNotExists() {
    int lastFolderResult = 0;
    int creationStatus = OBLI_PREFIX_OK;

    char *obliDirPath = _obli_genPrefixPath(0);
    lastFolderResult = _obli_createFolderIfNotExists(obliDirPath);
    if (lastFolderResult < 0) {
        return lastFolderResult;
    } else if (lastFolderResult == 0) {
        creationStatus = OBLI_PREFIX_CREATED;
    }

    free(obliDirPath);

    const char *subdirectories[] = {
        OBLI_MODULES_DIR,
        OBLI_PROGRAM_DIR,
        OBLI_NETWORK_DIR
    };
    for (int i = 0; i < LEN_ARRAY(subdirectories); i++) {
        char *newFolderPath = _obli_genPrefixPath(1, subdirectories[i]);

        lastFolderResult = _obli_createFolderIfNotExists(newFolderPath);
        if (lastFolderResult < 0) {
            return lastFolderResult;
        } else if ((lastFolderResult == 0) && (creationStatus != OBLI_PREFIX_CREATED)) {
            creationStatus = OBLI_PREFIX_POPULATED;
        }
    }

    return creationStatus;
}

unsigned int obli_hashModuleName(const char *key) {
    return murmurhash(key, 32, 88);
}

int obli_installModule(const char *moduleName) {
    char *modulesPath = _obli_genPrefixPath(1, OBLI_MODULES_DIR);
    int didCreateModulesDir = _obli_createFolderIfNotExists(modulesPath);
    if (didCreateModulesDir == -1)
        return -1;

    // figure out if module name is URL or just github project
    unsigned int lenUrl = strlen(moduleName) + 1;
    char *url = malloc(lenUrl);
    if (strncmp(moduleName, "git@", 4) != 0 &&
            strncmp(moduleName, "git://", 6) != 0 &&
            strncmp(moduleName, "http://", 7) != 0 &&
            strncmp(moduleName, "https://", 8) != 0) {
        lenUrl += 23;
        url = malloc(lenUrl);
        sprintf(url, "https://github.com/%s.git", moduleName);
    } else {
        strcpy(url, moduleName);
    }

    // use a hashing function to generate consistent and searchable directories
    char *newRepoPath = malloc(strlen(modulesPath) + strlen(moduleName) + 2);
    unsigned int nameHash = obli_hashModuleName(moduleName);
    sprintf(newRepoPath, "%s/%x", modulesPath, nameHash);

    // TODO: support SSH
    git_libgit2_init();
    git_repository *repo = NULL;
    git_clone_options *opts = malloc(sizeof(git_clone_options));
    git_clone_options_init(opts, GIT_CLONE_OPTIONS_VERSION);
    git_clone(&repo, url, newRepoPath, opts);
    git_repository_free(repo);

    free(opts);
    free(url);
    free(newRepoPath);
    _obli_freePrefixPath(modulesPath);

    return 0;
}

void obli_launchDaemon() {
    const char *obliDirPath = _obli_genPrefixPath(0);
    char *daemonPath = _obli_genPrefixPath(2, OBLI_PROGRAM_DIR, "oblid");
    char *socketPath = _obli_genPrefixPath(2, OBLI_NETWORK_DIR, "daemon.sock");

    sprintf(daemonPath, "%s/bin/oblid", obliDirPath);

    execle(daemonPath, "oblid", NULL);

    free(daemonPath);
    free(socketPath);
}