include sources.mk

CC=gcc
CFLAGS=-Wall -Werror -g -O0 -std=c99
LFLAGS=-Wl -Map=main.map

PROGRAMS=hw1
#LIBRARIES=cBuff.so

#Default make command builds executable file
all: hw1

hw1:  $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

hw1.so: circbuff.o linkedList.o
	$(LD) -shared circbuff.o linkedList.o -o hw1.so

#Creates an object file of corresponding C file
%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean

clean:
	rm -f $(PROGRAMS)
	rm -f *.o
	rm -f *.so
