#!/bin/bash

mkdir -p build 2> /dev/null

gcc -c src/aside.c -o build/aside.o -g3 -std=c99 -Wall -Wextra -Wpedantic -O1
gcc -c src/clear_button.c -o build/clear_button.o -g3 -std=c99 -Wall -Wextra -Wpedantic -O1
gcc -c src/coordinate.c -o build/coordinate.o -g3 -std=c99 -Wall -Wextra -Wpedantic -O1
gcc -c src/coordinate_system.c -o build/coordinate_system.o -g3 -std=c99 -Wall -Wextra -Wpedantic -O1
gcc -c src/legend.c -o build/legend.o -g3 -std=c99 -Wall -Wextra -Wpedantic -O1
gcc -c src/main.c -o build/main.o -g3 -std=c99 -Wall -Wextra -Wpedantic -O1
gcc -c src/utils.c -o build/utils.o -g3 -std=c99 -Wall -Wextra -Wpedantic -O1
gcc build/aside.o build/clear_button.o build/coordinate.o build/coordinate_system.o build/legend.o build/main.o build/utils.o -o relativity  -lm -lraylib
