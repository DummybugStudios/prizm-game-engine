CC = gcc
CFLAGS = $(shell pkg-config --cflags sdl2) -g -Wall
LDFLAGS = $(shell pkg-config --libs sdl2) -lm
CFILES = $(wildcard *.c)

.PHONY: all main opt clean profile

all: main opt

main: build build/main

build/main: $(CFILES)
	$(CC) $(CFILES) $(CFLAGS) -fsanitize=address $(LDFLAGS) -fsanitize=address -o $@

opt: build build/opt
build/opt: $(CFILES)
	$(CC) $^ $(CFLAGS) -O3 $(LDFLAGS) -o $@

profile: build build/profile
build/profile: $(CFILES)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

build:
	@mkdir -p $@

clean:
	rm -f build/main
	rm -f build/opt
	rmdir build/