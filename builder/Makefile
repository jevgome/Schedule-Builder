CC := gcc
CFLAGS := -g -Wall -std=c99
TARGETS := main

all: $(TARGETS)

main: main.c course.h course.c input.h input.c schedule.h schedule.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o $(TARGETS)
