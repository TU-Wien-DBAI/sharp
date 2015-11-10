#!/bin/sh
set -e

# create m4 folder if not present
if [ ! -d "m4" ]; then
	mkdir m4
fi

# create build-aux folder if not present
if [ ! -d "build-aux" ]; then
	mkdir build-aux
fi

# install autotools, generate configure script and Makefile.in
autoreconf -fvi
