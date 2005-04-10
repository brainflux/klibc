#!/bin/sh

# Copy klibc to kernel - integrating klibc with the kbuild infrastructure
# Most files are located in the usr/ directory structure in the kernel

# fetch kernel directory
if [ -z $1 ]; then
	echo "Copy klibc to kernel"
	echo "$0: path-to-kernel-src"
	exit 1
fi

kernel=$1
if [ ! -d $kernel ]; then
	echo "$kernel is not a kernel directory"
	exit 1
fi

if [ -z $2 ]; then
	echo "Copying source files"
	# 1) Copy all klibc source files
	if [ ! -d $kernel/usr/klibc ]; then
		mkdir $kernel/usr/klibc
	fi
	cp -R klibc/* $kernel/usr/klibc

	echo "Copying header files"
	# 2) And the include files
	if [ ! -d $kernel/usr/include ]; then
		mkdir $kernel/usr/include
	fi
	cp -R include/* $kernel/usr/include
fi

echo "Copying kbuild files"
cp kernel/Kbuild.*          $kernel/scripts
cp kernel/Kbuild            $kernel/usr
cp klibc/Kbuild             $kernel/usr/klibc
cp klibc/syscalls/Kbuild    $kernel/usr/klibc/syscalls
cp klibc/socketcalls/Kbuild $kernel/usr/klibc/socketcalls


