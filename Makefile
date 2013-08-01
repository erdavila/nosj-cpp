TARGET=nosj-test

_UNAME=$(shell uname -o)
ifeq ($(_UNAME), Cygwin)
  EXT=.exe
else
  EXT=
endif

NF=-nofork

INL=nosj.inl
H=nosj.hpp
SRC=$(TARGET).cpp

OBJ=$(TARGET).o
OBJ2=$(TARGET)-again.o
OBJS=$(OBJ) $(OBJ2)
EXE=$(TARGET)$(EXT)

OBJNF=$(TARGET)$(NF).o
OBJSNF=$(OBJNF) $(OBJ2)
EXENF=$(TARGET)$(NF)$(EXT)

CXXFLAGS=-Wall -O0 -g -std=c++11


.PHONY: all
all: $(EXE) $(EXENF)

.PHONY: clean
clean:
	rm -rf $(OBJS) $(OBJSNF) $(EXE) $(EXENF)

.PHONY: test
test: $(EXE)
	./$<
	
.PHONY: test-with-valgrind
test-with-valgrind: $(EXENF)
	valgrind --leak-check=full ./$<


$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $+ -o $@

$(EXENF): $(OBJSNF)
	$(CXX) $(CXXFLAGS) $+ -o $@


$(OBJ): $(SRC) $(H) $(INL)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJNF): $(SRC) $(H) $(INL)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -DNOFORK

$(OBJ2): $(SRC) $(H) $(INL)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -Dmain=pain
