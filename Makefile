CC		 = gcc
BIN		 = relativity
OBJ      = window.o coordinate.o utils.o
LINKOBJ  = window.o coordinate.o utils.o
CFLAGS   = -g3 -std=c99 -Wall -Wextra -Wpedantic -O1
RM       = rm.exe -f

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o $(BIN) $(LIBS) -lm -lraylib

window.o: window.c
	$(CC) -c window.c -o window.o $(CFLAGS)

coordinate.o: coordinate.c
	$(CC) -c coordinate.c -o coordinate.o $(CFLAGS)

utils.o: utils.c
	$(CC) -c utils.c -o utils.o $(CFLAGS)

clean:
	rm -f *.o