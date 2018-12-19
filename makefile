SOURCES=Board.cpp Engine.cpp Helper.cpp interface.cpp
EXECUTABLE=chess
CC=g++
CFLAGS=-Wall -std=c++11
LDFLAGS=
OBJECTS=$(SOURCES:.cpp=.o)

ifeq ($(PERF),1)
	CFLAGS+=-DPERF
	EXECUTABLE=perfchess
endif

all: $(SOURCES) $(EXECUTABLE)

	@echo $(CFLAGS)
	@echo $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o core
	rm -rf *~