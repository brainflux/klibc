#!/bin/sh
if [ -z "$RM" ]; then
  export RM='cg-rm -rf'
fi

nuke () {
    find "$@" -print | rev | xargs -rt $RM
}

nuke Kbuild Makefile defconfig klibc.spec.in *.sh
nuke contrib klcc

# These files are either not needed or provided from the
# kernel tree
nuke scripts/Kbuild.include scripts/Kbuild.install
nuke scripts/Makefile.*
nuke scripts/basic
