TARGET=nosj-test

_UNAME=$(shell uname -o)
ifeq ($(_UNAME), Cygwin)
  EXT=.exe
else
  EXT=
endif

ifeq ($(NOFORK), Yes)
  SUFF=-nofork
  NOFORKFLAG= -DNOFORK
else
  SUFF=
  NOFORKFLAG=
endif


H=nosj.hpp
SRC=$(TARGET).cpp
OBJ=$(TARGET)$(SUFF).o
OBJ2=$(TARGET)2$(SUFF).o
OBJS=$(OBJ) $(OBJ2)
EXE=$(TARGET)$(SUFF)$(EXT)

CXXFLAGS=-Wall -O0 -g -std=c++11 $(NOFORKFLAG)


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
	valgrind --leak-check=full ./$<


$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $+ -o $@

$(OBJ): $(SRC) $(H)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ2): $(SRC) $(H)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -Dmain=pain
