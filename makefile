CC=g++
FLAGS=-Wall -pedantic -g -std=c++11
LINKS=-lpcap
OUT=sniff
ARGS=cap-feb-6
FILES=$(wildcard *.cpp)
OBJS=$(FILES:.cpp=.o)
HEADERS=$(wildcard *.h)

$(OUT): $(OBJS)
	$(CC) $(FLAGS) $^ -o $@ $(LINKS)


$(OBJS): $(FILES) $(HEADERS)
	$(CC) $(FLAGS) -c $(filter %.cpp,$^)

%.h:

clean:
	rm $(OUT) *.o

run: $(OUT)
	./$(OUT) $(ARGS)

debug:
	gdb --args $(OUT) $(ARGS)
