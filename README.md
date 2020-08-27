# obli (cli)

Before, you might have had to run two separate servers to give obli all the functionality it needed. Obli 2 plans to eliminate that.

## Dependencies

For this to compile, your system will need the following:

1. [libgit2](https://libgit2.org/) (tested with v1.0.1)
2. upx (dist only)

## How To Build

You can build this by running `make` or `make dist` (which will strip debug information and compress the daemon executable).

## Overall obli2 Feature Wishlist

1. Collaborative editing of files if you share an organization
2. Cleaner UI
3. Server-side modules should be able to be written in any language
4. Command line-centric interface (both server and browser are started/stopped from the command line)
5. Scripts that can require certain permissions (selenium, etc.) or dispatcher modules