LIBS = OGRE OIS
CXX = g++
CXXFLAGS = -Wall -Werror $(shell pkg-config --cflags $(LIBS))
OBJS = InputSystem.o Locator.o Application.o main.o
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

clean:
	rm -rf SegwaySlaughter *~ *.o
