
SRC=mag3110.c
TARGET=mag3110
 
CC ?= gcc
CFLAGS ?= -O2 -Wall -g



all:
	$(CROSS_COMPILE)gcc $(CFLAGS) $(SRC) -o $(TARGET)
	
clean:
	rm -rf $(TARGET)