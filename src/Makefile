CXXFLAGS = -Wall -ggdb

OBJS :=  pipeline.o utils.o simulator.o main.o assembler.o language.o Processor.o \
	CodeMemory.o DataMemory.o CPUConfig.o

all: mips64

mips64: $(OBJS)
	c++ -o mips64 $(OBJS) -lcurses

pipeline.o: pipeline.cpp utils.h mytypes.h

utils.o: utils.h utils.cpp mytypes.h

simulator.o: simulator.cpp simulator.h mytypes.h utils.h

main.o: main.cpp simulator.h mytypes.h utils.h

clean:
	rm -rf *.o mips64
