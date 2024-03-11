
# The compiler
CC:=gcc
# The flags passed to the compiler
CFLAGS:=-g -Ofast -Wall -Wextra -fsanitize=address -I/opt/homebrew/include -I/opt/homebrew/include/SDL2
# The flags passed to the linker
LDFLAGS:=-lm -L/opt/homebrew/lib -lSDL2 # -lSDL2_ttf -lSDL2_image

# Path to the lib Vec2
VPATH:=./utils/vec2 ./utils/gfx 

main: main.o vec2.o gfx.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

vec2.o: vec2.h

gfx.o: gfx.h

clean:
	rm -f *.o main tests
