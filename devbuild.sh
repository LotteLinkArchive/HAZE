#!/bin/bash
if [ ! -d "./builddir" ]; then
	CC=gcc meson builddir
fi

cd builddir
meson configure -Dc_args="-Ofast -march=native -mtune=native -g"
cd ..

meson compile -C builddir
