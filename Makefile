TARGET := nosj-test

_UNAME := $(shell uname -o)
ifeq ($(_UNAME), Cygwin)
  EXT := .exe
else
  EXT :=
endif

NF := -nofork

SRC_MAIN := $(TARGET).cpp
SRCS     := $(filter-out $(SRC_MAIN), $(wildcard *.cpp))

OBJ_MAIN   := $(TARGET).o
OBJS       := $(SRCS:.cpp=.o)
OBJS_AGAIN := $(patsubst %.o, %-again.o, $(OBJ_MAIN) $(OBJS))
OBJ_MAINNF := $(TARGET)$(NF).o
ALL_OBJS   := $(OBJ_MAIN) $(OBJS) $(OBJS_AGAIN) $(OBJ_MAINNF)

EXE    := $(TARGET)$(EXT)
EXENF  := $(TARGET)$(NF)$(EXT)
EXELNK := linkage-for-redefinition-detection


CXXFLAGS := -Wall -O0 -g -std=c++11

MAKEDEPS = @g++ $(CXXFLAGS) -MM $< -o $(@:.o=.d) -MT $@ -MP
COMPILE  =  g++ $(CXXFLAGS) -c  $< -o $@
LINK     =  g++ $(CXXFLAGS) $+ -o $@


.PHONY: all
all: $(EXE) $(EXENF) $(EXELNK)

.PHONY: clean
clean:
	rm -f *.d *.o $(EXE) $(EXENF) $(EXELNK)

.PHONY: test
test: $(EXE)
	./$<

.PHONY: test-with-valgrind
test-with-valgrind: $(EXENF)
	valgrind --leak-check=full ./$<


$(EXE): $(OBJ_MAIN) $(OBJS)
	$(LINK)

$(EXENF): $(OBJ_MAINNF) $(OBJS)
	$(LINK)

$(EXELNK): $(OBJ_MAIN) $(OBJS) $(OBJS_AGAIN)
	$(LINK)
	@chmod -x $@


%.o: %.cpp
	$(MAKEDEPS)
	$(COMPILE)

%-nofork.o: %.cpp
	$(MAKEDEPS) -DNOFORK
	$(COMPILE)  -DNOFORK

%-again.o: %.cpp
	$(MAKEDEPS) -Dmain=pain -Dtests=pests
	$(COMPILE)  -Dmain=pain -Dtests=pests


-include $(ALL_OBJS:.o=.d)
