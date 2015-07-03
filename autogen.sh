#!/bin/sh

if [ ! -d "m4" ]; then
	mkdir m4
fi

if [ ! -d "autotools" ]; then
	mkdir autotools
fi

autoreconf -fvi
