#!/bin/bash -x
#
# Make a tarball from the current CVS tree
#

if [ -z "$CVS" ]; then
    CVS=cvs
fi
export CVSROOT=`cat CVS/Root`

tmp=/var/tmp/klibc.$$
rm -rf $tmp
mkdir -p $tmp

cd $tmp

$CVS export -D now klibc
rm -f klibc/maketar.sh
( cd klibc && make klibc.spec )
version=`cat klibc/version`
mv klibc klibc-$version
rm -f ../klibc-$version.tar*
tar cvvf ../klibc-$version.tar klibc-$version
gzip -9 ../klibc-$version.tar
cd ..
rm -rf $tmp

