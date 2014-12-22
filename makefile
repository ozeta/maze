CC=gcc
CLANG=clang
CPP=g++
CFLAGS = -lm -g
all: main.c ./src/*.c
	
	$(CLANG) -o labirinto main.c ./src/*.c $(CFLAGS)

gcc: main.c ./src/*.c
	$(CC) -o labirinto main.c ./src/*.c $(CFLAGS)

g++: main.c ./src/*.c
	$(CPP) -o labirinto main.c ./src/*.c $(CFLAGS) -Wno-write-strings

clang: main.c ./src/*.c
	$(CLANG) -o labirinto main.c ./src/*.c $(CFLAGS) 

wextra: main.c ./src/*.c
	$(CC) -o labirinto main.c ./src/*.c $(CFLAGS) -Wextra -Wstrict-prototypes -Wconversion -Wunreachable-code -Wall -Wuninitialized

clean:
	find ./ -type f -exec touch {} +