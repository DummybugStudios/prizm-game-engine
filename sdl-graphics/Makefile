CC = gcc
CFLAGS = $(shell pkg-config --cflags sdl2) -g -Wall
LDFLAGS = $(shell pkg-config --libs sdl2)
CFILES = $(wildcard *.c)

build/main: build $(CFILES)
	$(CC) $(CFILES) $(CFLAGS) -fsanitize=address  $(LDFLAGS) -fsanitize=address -static-libasan -o $@

build:
	@mkdir -p $@

build/opt: $(CFILES)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

