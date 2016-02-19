CC=g++
FLAGS=-Wall -pedantic -g -std=c++11
LINKS=-lpcap
OUT=sniff
OBJS=main.o packet.o connection.o
ARGS=cap-feb-6
FILES=main.cpp packet.cpp connection.cpp
HEADERS=connection.h states.h packet.h

$(OUT): $(OBJS)
	$(CC) $(FLAGS) $^ -o $@ $(LINKS)


$(OBJS): $(FILES) $(HEADERS)
	$(CC) $(FLAGS) -c $(filter %.cpp,$^)

%.h:

clean:
	rm $(OUT) *.o

run: 
	./$(OUT) $(ARGS)

debug:
	gdb --args $(OUT) $(ARGS)
