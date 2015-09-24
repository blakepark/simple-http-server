PROJECT_PATH 	= .
INCLUDE_PATH  	= $(PROJECT_PATH)/include
SOURCE_PATH 	= $(PROJECT_PATH)/src
BIN		= simple-http-server
CC      	= /usr/bin/gcc
CFLAGS		= -lpthread -I $(INCLUDE_PATH)

simple-http-server: main.c $(SOURCE_PATH)/error.c
	$(CC) -o $(BIN) main.c $(SOURCE_PATH)/error.c $(CFLAGS)

clean:
	rm ./simple-http-server
