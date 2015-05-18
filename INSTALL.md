# How to install MacGitver

In present state, you need to compile MacGitver from source. We provide a compilable version from our default branch `development`. If you like to help with creating binary packages, please create a [pull request](https://github.com/macgitver/MacGitver/pulls) or contact me via [mail](mailto:nils@macgitver.org).

## Prerequisites

To be able to build and install, the following packages are required:

* Qt >=5.4
* CMake >=3.1
* C++11 (libc++)
* A recent C++ compiler GCC (>=4.9) or Clang (>=3.6)
* Git (must support submodules)

If you have questions on how to install those packages, feel free to open an issue or write e-mail.

## Install

Depending on your hardware, the compile will take about 5-15 minutes.

## Linux / OSX

After you installed the above mentioned packages, open a terminal and cd to your favourite directory. Example:

```bash
cd ~/Projects/3rd-party
```

From here, clone the MacGitver repository and run the install script:

```bash
git clone git://github.com/macgitver/MacGitver
cd MacGitver/scripts
./mgv-dev-install.sh
```

The script leads you through the installation process. MacGitver is installed completely into a single directory.

You can now start MacGitver: `~/Projects/MacGitver-Release/installation/bin/MacGitver`

## Windows

Not supported yet.

# How to uninstall MacGitver

If you need to uninstall MacGitver for whatever reason, simply remove the installation folder and optionally the configuration folder.

## Linux

```bash
rm -rf ~/Projects/MacGitver-Release

# Want to get rid of the configuration file(s) as well?
rm -rf ~/.config/MacGitver

# Last not least, remove  the directory, where you cloned the sources. For the above example:
rm -rf ~/Projects/3rd-party/MacGitver
```

## OSX

Not available yet ...

## Windows

Not available yet ...