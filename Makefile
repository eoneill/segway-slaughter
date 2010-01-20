LIBS = OGRE OIS
CXX = g++
CXXFLAGS = -Wall -Werror $(shell pkg-config --cflags $(LIBS))
OBJS = OgreEngineManager.o main.o
LDFLAGS = $(shell pkg-config --libs $(LIBS))

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o SegwaySlaughter $(OBJS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

OgreEngineManager.o: OgreEngineManager.h OgreEngineManager.cpp
	$(CXX) $(CXXFLAGS) -c OgreEngineManager.cpp

clean:
	rm -rf SegwaySlaughter *~
