CC=gcc
CFLAGS=-Wall -pedantic -Werror -g 
LDFLAGS=-lm
RM=rm -f
SOURCES=$(wildcard *.c)
TARGETS=$(SOURCES:.c=)
OBJECTS=$(SOURCES:.c=.o)

.PHONY: all clean

all: counter sloppy_counter hand_list

%: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@  $< 

counter: counter.o
	$(CC) $(CFLAGS) $^ -o $@

sloppy_counter: sloppy_counter.o
	$(CC) $(CFLAGS) $^ -o $@

hand_list: hand_list.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(OBJECTS) $(TARGETS)