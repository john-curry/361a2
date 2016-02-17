#makefile for project 1 works both in linux and unix system now
CC=g++
FLAGS=-Wall -pedantic -g -std=c++11
LINKS=
OUT=sniff
FILES=main.cpp
# compile client and server
all:$(OUT)
	ctags *.cpp

$(OUT): $(FILES)
	$(CC) $(FLAGS) -o $@ $^ $(FLAGS)

clean:
	rm $(OBJCLIENT) $(OBJSERVER) $(OUT)

configuredebug: all
	cat client.c | sed 's/DEBUG [0-1]/DEBUG 1/'
	cat server.c | sed 's/DEBUG [0-1]/DEBUG 1/'

cdebug:
	gdb --args $(CLIENT) http://www.example.com:80/index.html

sdebug:
	gdb --args $(SERVER) 8000 .

crun: 
	./$(CLIENT) http://www.example.com:80/index.html

srun:
	./$(SERVER) 8000 .
