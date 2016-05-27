CC=clang
CFLAGS = -std=c99 -Wall -Wextra
LDFLAGS = # -lSDL    # si besoin pour le framebuffer...

all: main

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
main: main.o operations.o gpr.o
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -rf *.o main
