LIBS = OGRE OIS CEGUI CEGUI-OGRE openal freealut
CXX = g++
CXXFLAGS = -Wall -Werror -ansi $(shell pkg-config --cflags $(LIBS))
OBJS = audio.o \
	     Actor.o \
	     InputSystem.o \
       Locator.o \
       GuiListener.o \
       SideScroller.o \
       Application.o \
	     CasinoLevel.o \
	     MainMenu.o \
       HUD.o \
	     main.o

LDFLAGS = $(shell pkg-config --libs $(LIBS))

all: $(OBJS)
	@echo $(LDFLAGS)
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

AIManager.o: AIManager.h AIManager.cpp
	$(CXX) $(CXXFLAGS) -c AIManager.cpp

MainMenu.o: MainMenu.h MainMenu.cpp
	$(CXX) $(CXXFLAGS) -c MainMenu.cpp

GuiListener.o: GuiListener.h GuiListener.cpp
	$(CXX) $(CXXFLAGS) -c GuiListener.cpp

Actor.o: Actor.h Actor.cpp
	$(CXX) $(CXXFLAGS) -c Actor.cpp

HUD.o: HUD.h HUD.cpp
	$(CXX) $(CXXFLAGS) -c HUD.cpp

clean:
	rm -rf SegwaySlaughter *~ *.o

remake: clean all
