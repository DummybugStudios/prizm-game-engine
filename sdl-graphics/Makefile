CC = gcc
CFLAGS = $(shell pkg-config --cflags sdl2) -g -Wall
LDFLAGS = $(shell pkg-config --libs sdl2) -lm
CFILES = $(wildcard *.c)

build/main: build $(CFILES)
	$(CC) $(CFILES) $(CFLAGS) -fsanitize=address  $(LDFLAGS) -fsanitize=address -o $@

opt: build build/opt

build:
	@mkdir -p $@

build/opt: $(CFILES)
	$(CC) $^ $(CFLAGS) -O3 $(LDFLAGS) -o $@