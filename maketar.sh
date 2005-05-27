#!/bin/bash -x
#
# Make a tarball from the current git repository
#

tmp=/var/tmp/klibc.$$
rm -rf $tmp
cg-export $tmp
rm -f klibc/maketar.sh
( cd klibc && make klibc.spec )
version=`cat klibc/version`
mv klibc klibc-$version
rm -f ../klibc-$version.tar*
tar cvvf ../klibc-$version.tar klibc-$version
gzip -9 ../klibc-$version.tar
cd ..
rm -rf $tmp

