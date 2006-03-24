#!/bin/bash -xe
#
# Make a tarball from the current git repository
#

[ -z "$tmpdir" ] && tmpdir=/var/tmp

tmp=$tmpdir/klibc.$$
rm -rf $tmp
cg-export $tmp
cd $tmp
make release
version=`cat version`
rm -rf $tmpdir/klibc-$version
mv $tmp $tmpdir/klibc-$version
cd ..
rm -f klibc-$version.tar*
tar cvvf klibc-$version.tar klibc-$version
gzip -9 klibc-$version.tar
rm -rf klibc-$version
