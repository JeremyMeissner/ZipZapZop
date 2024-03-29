
# The compiler
CC:=gcc
# The flags passed to the compiler
CFLAGS:=-g -Ofast -Wall -Wextra -fsanitize=address -I/opt/homebrew/include -I/opt/homebrew/include/SDL2
# The flags passed to the linker
LDFLAGS:=-lm -L/opt/homebrew/lib -lSDL2 # -lSDL2_ttf -lSDL2_image

# Path to the libs
VPATH:=./utils/vec2 ./utils/gfx ./utils/charge

main: main.o vec2.o gfx.o charge.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: main
	rm -f *.o
	./main

clean:
	rm -f *.o main 
