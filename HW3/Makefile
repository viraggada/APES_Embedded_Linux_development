CC=gcc
CFLAGS=-Wall -Werror -g -O0 -pthread
LFLAGS=-Wl -Map=main.map

SSRCS=q5.c

PROGRAMS=q5

#Default make command builds executable file
all: $(PROGRAMS)

q5: $(SSRCS)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean

clean:
	rm -rf $(PROGRAMS)
