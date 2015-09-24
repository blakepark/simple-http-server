PROJECT_PATH 	= .
INCLUDE_PATH  	= $(PROJECT_PATH)/include
SOURCE_PATH 	= $(PROJECT_PATH)/src
BIN		= simple-http-server
CC      	= /usr/bin/gcc
CFLAGS		= -lpthread -I $(INCLUDE_PATH)

simple-http-server: $(SOURCE_PATH)/main.c $(SOURCE_PATH)/error.c $(SOURCE_PATH)/server.c
	$(CC) -o $(BIN) $(SOURCE_PATH)/main.c $(SOURCE_PATH)/server.c $(SOURCE_PATH)/error.c $(CFLAGS)

clean:
	rm ./simple-http-server
