TARGET=nosj-test

_UNAME=$(shell uname -o)
ifeq ( $(_UNAME), Cygwin)
  EXT=.exe
else
  EXT=
endif


H=nosj.hpp
SRC=$(TARGET).cpp
OBJ=$(TARGET).o
OBJ2=$(TARGET)2.o
OBJS=$(OBJ) $(OBJ2)
EXE=$(TARGET)$(EXT)

CXXFLAGS=-Wall -O0 -g


.PHONY: all
all: $(EXE)

.PHONY: clean
clean:
	rm -rf $(OBJS) $(EXE)

.PHONY: test
test: $(EXE)
	./$<
	
.PHONY: test-with-valgrind
test-with-valgrind: $(EXE)
	valgrind ./$<


$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $+ -o $@

$(OBJ): $(SRC) $(H)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ2): $(SRC) $(H)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -Dmain=pain
