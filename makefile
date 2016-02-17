CC=g++
FLAGS=-Wall -pedantic -g -std=c++11
LINKS=-lpcap
OUT=sniff
OBJS=main.o

FILES=main.cpp
HEADERS=connection.h states.h packet.h

$(OUT): $(OBJS)
	$(CC) $(FLAGS) -o $@ $^ $(LINKS)

-include $(OBJS:.o=.d)

$(OBJS): $(FILES) $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm $(OUT) *.o
