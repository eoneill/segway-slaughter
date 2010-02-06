LIBS = OGRE OIS openal #freealut
CXX = g++
CXXFLAGS = -Wall -Werror $(shell pkg-config --cflags $(LIBS))
OBJS = InputSystem.o Locator.o SideScroller.o Application.o main.o #CasinoLevel.o
LDFLAGS = $(shell pkg-config --libs $(LIBS))

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o SegwaySlaughter $(OBJS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

InputSystem.o: InputSystem.h InputSystem.cpp
	$(CXX) $(CXXFLAGS) -c InputSystem.cpp

Locator.o: Locator.h Locator.cpp
	$(CXX) $(CXXFLAGS) -c Locator.cpp

Application.o: Application.h Application.cpp
	$(CXX) $(CXXFLAGS) -c Application.cpp

SideScroller.o: SideScroller.h SideScroller.cpp
	$(CXX) $(CXXFLAGS) -c SideScroller.cpp

#CasinoLevel.o: CasinoLevel.h CasinoLevel.cpp
#	$(CXX) $(CXXFLAGS) -c CasinoLevel.cpp

clean:
	rm -rf SegwaySlaughter *~ *.o

remake: clean all
