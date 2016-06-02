
SRC=mag3110.c i2c/smbus.c
TARGET=mag3110

CC ?= gcc
INC = -Iinclude/
CFLAGS = -O0 -Wall -g


$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INC) $(SRC) -o $(TARGET)


.PHONY: all
all: $(TARGET)
	
	
.PHONY: clean
clean:
	rm -rf $(TARGET)