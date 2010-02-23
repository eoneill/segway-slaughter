LIBS = OGRE OIS CEGUI CEGUI-OGRE openal freealut
CXX = g++
CXXFLAGS = -Wall -Werror -ansi $(shell pkg-config --cflags $(LIBS))
OBJS = audio.o \
	     InputSystem.o \
       Locator.o \
       GuiListener.o \
       SideScroller.o \
       Application.o \
	     CasinoLevel.o \
       MainMenu.o \
       main.o
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

CasinoLevel.o: CasinoLevel.h CasinoLevel.cpp
	$(CXX) $(CXXFLAGS) -c CasinoLevel.cpp

audio.o: audio/audio.h audio/audio.cpp
	$(CXX) $(CXXFLAGS) -c audio/audio.cpp -o audio.o

MainMenu.o: MainMenu.h MainMenu.cpp
	$(CXX) $(CXXFLAGS) -c MainMenu.cpp

GuiListener.o: GuiListener.h GuiListener.cpp
	$(CXX) $(CXXFLAGS) -c GuiListener.cpp

clean:
	rm -rf SegwaySlaughter *~ *.o

remake: clean all
