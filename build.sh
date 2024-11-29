#!/bin/bash
gcc -c aside.c -o aside.o -g3 -std=c99 -Wall -Wextra -Wpedantic -O1
gcc -c clear_button.c -o clear_button.o -g3 -std=c99 -Wall -Wextra -Wpedantic -O1
gcc -c utils.c -o utils.o -g3 -std=c99 -Wall -Wextra -Wpedantic -O1
gcc -c coordinate.c -o coordinate.o -g3 -std=c99 -Wall -Wextra -Wpedantic -O1
gcc -c main.c -o main.o -g3 -std=c99 -Wall -Wextra -Wpedantic -O1 `pkg-config --libs raylib` -lm
gcc main.o coordinate.o utils.o clear_button.o -o relativity -g3 -std=c99 -Wall -Wextra -Wpedantic -O1 `pkg-config --libs raylib` -lm