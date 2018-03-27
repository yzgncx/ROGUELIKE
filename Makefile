CC=g++

CFLAGS= -c -std=c++11 

LDFLAGS=-lncursesw -lmenu

EXECUTABLE=game

all: $(EXECUTABLE)

debug: CFLAGS += -g
debug: $(EXECUTABLE)

SOURCES= GameWindow.cpp main.cpp Map.cpp utilities.cpp Actor.cpp Attribute.cpp Game.cpp Menu.cpp Stats.cpp 

OBJECTS=$(SOURCES:.cpp=.o)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(EXECUTABLE) $(DEBUG)
