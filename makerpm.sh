#!/bin/bash -xe
#
# Make an rpm from the current git repository
#

[ -z "$tmpdir" ] && export tmpdir=/var/tmp

./maketar.sh
rpmbuild -ta klibc-`cat version`.tar.gz
