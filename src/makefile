#include directories
INCLUDES+=. card_list TurtleGUI utilities

#libraries
#the order of the $(LIBS) is important, at least for MinGW
LIBS+=common.a libturtlegui.a
ifeq ($(OS),Windows_NT)
	LIBS+=-lmingw32
endif
LIBS+=-lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

#flags
CXXFLAGS+=-std=c++11 $(addprefix -I,$(INCLUDES)) -fpermissive -w
ifeq ($(shell uname), Linux)
	#read data about the current install
	CXXFLAGS+=$(shell sdl-config --cflags --static-libs)
endif

#source
CXXSRC=$(wildcard *.cpp)

#objects
OBJDIR=obj
OBJ+=$(addprefix $(OBJDIR)/,$(CXXSRC:.cpp=.o))

#output
OUTDIR=../out
OUT=$(addprefix $(OUTDIR)/,labyrinth)

#targets
all: $(OBJ) $(OUT)
	$(MAKE) -C card_list
	$(MAKE) -C TurtleGUI
	$(MAKE) -C utilities
#	$(CXX) $(CXXFLAGS) -o $(OUT) $(OBJ) $(LIBS)
	$(CXX) $(CXXFLAGS) -o $(OUTDIR)/card_renderer card_renderer.cpp obj/render_trading_card.o obj/trading_card.o $(LIBS)

$(OBJ): | $(OBJDIR)

$(OUT): | $(OUTDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OUTDIR):
	mkdir $(OUTDIR)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
ifeq ($(OS),Windows_NT)
	$(RM) *.o *.a *.exe
else ifeq ($(shell uname), Linux)
	find . -type f -name '*.o' -exec rm -f -r -v {} \;
	find . -type f -name '*.a' -exec rm -f -r -v {} \;
	rm -f -v $(OUT)
endif

rebuild: clean all
