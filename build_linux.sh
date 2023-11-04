#!/bin/sh

gcc -Wall -Isrc/external/include src/x11/*.c src/*.c src/external/src/*.c \
    -o game -lGL -lX11 -lm
