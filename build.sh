#!/bin/bash
gcc -c coordinate.c -o coordinate.o -g3 -std=c99 -Wall -Wextra -Wpedantic -O1
gcc -c window.c -o window.o -g3 -std=c99 -Wall -Wextra -Wpedantic -O1 `pkg-config --libs raylib` -lm
gcc window.o coordinate.o -o relativity -g3 -std=c99 -Wall -Wextra -Wpedantic -O1 `pkg-config --libs raylib` -lm