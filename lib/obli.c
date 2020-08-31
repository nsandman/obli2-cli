#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

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

    _obli_freePrefixPath(obliDirPath);

    const char *subdirectories[] = {
        OBLI_DIR_MODULES,
        OBLI_DIR_PROGRAM,
        OBLI_DIR_NETWORK,
        OBLI_DIR_TMP,
        OBLI_DIR_LOG
    };
    for (int i = 0; i < LEN_ARRAY(subdirectories); i++) {
        char *newFolderPath = _obli_genPrefixPath(1, subdirectories[i]);

        lastFolderResult = _obli_createFolderIfNotExists(newFolderPath);
        if (lastFolderResult < 0) {
            return lastFolderResult;
        } else if ((lastFolderResult == 0) && (creationStatus != OBLI_PREFIX_CREATED)) {
            creationStatus = OBLI_PREFIX_POPULATED;
        }

        _obli_freePrefixPath(newFolderPath);
    }

    return creationStatus;
}

unsigned int obli_hashModuleName(const char *key) {
    return murmurhash(key, 32, 88);
}

int obli_installModule(const char *moduleName) {
    char *modulesPath = _obli_genPrefixPath(1, OBLI_DIR_MODULES);
    int didCreateModulesDir = _obli_createFolderIfNotExists(modulesPath);
    if (didCreateModulesDir == -1)
        return -1;

    // figure out if module name is URL or just github project
    unsigned int lenUrl = strlen(moduleName) + 1;
    char *url = malloc(lenUrl);
    if (strncmp(moduleName, "git@", 4) != 0 &&
            strncmp(moduleName, "git://", 6) != 0 &&
            strncmp(moduleName, "http://", 7) != 0 &&
            strncmp(moduleName, "https://", 8) != 0) {      // is not url
        lenUrl += 23;
        url = malloc(lenUrl);
        sprintf(url, "https://github.com/%s.git", moduleName);
    } else {
        strcpy(url, moduleName);                            // is url
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

bool obli_isDaemonRunning() {
    char *pidFile = _obli_genPrefixPath(1, OBLI_FILE_DAEMON);
    bool res = _obli_isDaemonRunning(pidFile);
    _obli_freePrefixPath(pidFile);

    return res;
}

/** LAUNCH OBLID **/
// NOTE: does not check if oblid is running already
int obli_launchDaemon() {
    int pid = fork();
    if (pid > 0) {         // parent: save child process id to file and exit 
        char *pidFile = _obli_genPrefixPath(1, OBLI_FILE_DAEMON);

        FILE *f = fopen(pidFile, "w");
        fprintf(f, "%d", pid);
        fclose(f);

        _obli_freePrefixPath(pidFile);
    } else {               // we're in the child process, set this up as a daemon
        if (setsid() < 0)
            return -2;

        // TODO set stdout and stderr to log files in prefix

        char *daemonPath = _obli_genPrefixPath(2, OBLI_DIR_PROGRAM, "oblid");
        char *socketPath = _obli_genPrefixPath(1, OBLI_DIR_NETWORK);
        
        // hand control of this process over to {prefix}/bin/oblid
        execl(daemonPath, OBLID_PROCESS_NAME, socketPath, NULL);

        _obli_freePrefixPath(daemonPath);
        _obli_freePrefixPath(socketPath);
    }

    return pid;
}

/** KILL OBLID **/
void obli_killDaemon() {
    char *pidFile = _obli_genPrefixPath(1, OBLI_FILE_DAEMON);
    FILE *f = fopen(pidFile, "r");
    if (f == NULL)
        return;

    int pid;
    fscanf(f, "%d", &pid);
    fclose(f);

    kill(pid, SIGKILL);          // kill int pid read from file
    remove(pidFile);
    _obli_freePrefixPath(pidFile);
}

/** PRIVATE functions **/
bool _obli_isDaemonRunning(const char *path) {
    return (access(path, F_OK) != -1);          // if pidFile exists, assume we are already running             
}