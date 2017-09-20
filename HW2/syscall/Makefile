CC=gcc
CFLAGS=-Wall -Werror -g
#-O0
LFLAGS=-Wl -Map=main.map

SRC=userspace.c

PROGRAMS=test_syscall

#Default make command builds executable file
all: $(PROGRAMS)

test_syscall: $(SRC)
	$(CC) $(CFLAGS) $^ -o $@


.PHONY: clean

clean:
	rm -f $(PROGRAMS)

