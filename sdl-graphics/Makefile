CC = gcc
CFLAGS = $(shell pkg-config --cflags sdl2) $(shell pkg-config --cflags SDL2_gfx) -g -Wall -I../include
LDFLAGS = $(shell pkg-config --libs-only-L sdl2) $(shell pkg-config --libs-only-L SDL2_gfx)
LDLIBS  = $(shell pkg-config --libs-only-l sdl2) $(shell pkg-config --libs-only-l SDL2_gfx) -lm

# TODO: fix this stupid ass method please
CFILES := hgrid.c uniform_grid_collision.c utils.c constants.c
CFILES := $(addprefix ../src/, $(CFILES))
CFILES := $(wildcard *.c) $(CFILES)

.PHONY: all main opt clean profile

main: build build/main

all: main opt profile

build/main: $(CFILES)
	$(CC) $(CFLAGS) -fsanitize=address $(LDFLAGS) $(CFILES) $(LDLIBS) -fsanitize=address -o $@

opt: build build/opt
build/opt: $(CFILES)
	$(CC) $(CFLAGS) -O3 $(LDFLAGS) $^ $(LDLIBS) -o $@

profile: build build/profile
build/profile: $(CFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^  $(LDLIBS) -o $@

build:
	@mkdir -p $@

clean:
	rm -f build/main
	rm -f build/opt
	rmdir build/