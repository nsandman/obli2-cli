# obli (cli)

Before, you might have had to run two separate servers to give obli all the functionality it needed. Obli 2 plans to eliminate that.

## Feature Wishlist

1. Cleaner UI
2. Server-side modules should be able to be written in any language
3. Command line-centric interface (both server and browser are started/stopped from the command line)

## Dependencies

For this to compile, your system will need the following:

1. [libgit2](https://libgit2.org/) (tested with v1.0.1)
2. WebDriver for your browser ([Chrome](https://chromedriver.chromium.org/downloads), [Firefox](https://github.com/mozilla/geckodriver/releases), etc.) NOTE: Chrome is highly recommended, as this is what we test on
3. upx (when building dist only)

## How To Build

You can build this by running `make` or `make dist` (which will strip debug information and compress the daemon executable).
