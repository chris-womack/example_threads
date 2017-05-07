INC_DIR=/home/redbeard/dev/threads

threads: threads.c $(INC_DIR)/util/util.c
	gcc -Wall -Wextra threads.c $(INC_DIR)/util/util.c -I$(INC_DIR) -lpthread -o threads.o

all: threads

clean:
	rm -rf *.o	
