CXX = clang++
DEBUG_ARG = -g
DEBUG_ARG = 

SDL_INC = ../third_party/SDL2/include
SDL_LIB = ../third_party/SDL2/lib/x64

SRC_FILES = $(wildcard *.cpp)
HEADER_FILES = $(wildcard *.h)
OBJS = $(SRC_FILES:.cpp=.o)

CXXFLAGS += -I$(SDL_INC)
CXXFLAGS += -MMD -MP
CXXFLAGS += $(DEBUG_ARG)

LINKERFLAGS += -L$(SDL_LIB)
LINKERFLAGS += -lSDL2 -lSDL2main -lSDL2_ttf

.PHONEY: clean all

all : Tetris.exe

Tetris.exe: $(OBJS)
	$(CXX) $^ $(CXXFLAGS) $(LINKERFLAGS) -o $@

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c  $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del *.o *.exe *.pdb *.ilk *.d

-include $(OBJS:.o=.d)
