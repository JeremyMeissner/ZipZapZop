
# The compiler
CC:=gcc
# The flags passed to the compiler
CFLAGS:=-g -Ofast -Wall -Wextra -fsanitize=address -I/opt/homebrew/include -I/opt/homebrew/include/SDL2
# The flags passed to the linker
LDFLAGS:=-lm -L/opt/homebrew/lib -lSDL2 # -lSDL2_ttf -lSDL2_image

# Path to the lib Vec2
VPATH:=vec2 gfx planet

main: main.o vec2.o gfx.o planet.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run_tests: tests
	./$<

tests: vec_tests.o vec2.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

planet.o: planet.h

vec2.o: vec2.h

gfx.o: gfx.h

clean:
	rm -f *.o main tests
