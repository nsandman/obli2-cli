#include <string.h>
#include <stdlib.h>

#include <git2.h>
#include <git2/clone.h>

#include "obli.h"

int obli_installModule(const char *moduleName) {
    const char *modulesPath = _obli_genPrefixPath(1, OBLI_MODULES_DIR);
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

    // TODO: instead of being cloned into moduleName, use a hashing function to gen directory name
    char *newRepoPath = malloc(strlen(modulesPath) + strlen(moduleName) + 2);
    sprintf(newRepoPath, "%s/%s", modulesPath, moduleName);

    git_libgit2_init();

    // TODO: support SSH
    git_repository *repo = NULL;
    git_clone_options *opts = malloc(sizeof(git_clone_options));
    git_clone_options_init(opts, GIT_CLONE_OPTIONS_VERSION);
    int ret = git_clone(&repo, url, newRepoPath, opts);
    git_repository_free(repo);

    free(opts);
    free(url);
    free(newRepoPath);
    free(modulesPath);

    return 0;
}