CC		 = gcc
BIN		 = relativity
OBJ      = window.o coordinate.o
LINKOBJ  = window.o coordinate.o
CFLAGS   = -g3 -std=c99 -Wall -Wextra -Wpedantic -O1
RM       = rm.exe -f

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o $(BIN) $(LIBS) -lm -lraylib

window.o: window.c
	$(CC) -c window.c -o window.o $(CFLAGS)

coordinate.o: coordinate.c
	$(CC) -c coordinate.c -o coordinate.o $(CFLAGS)

clean:
	rm -f *.o