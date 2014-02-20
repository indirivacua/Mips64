CXXFLAGS = -Wall

all: mips64

mips64: pipeline.o utils.o simulator.o main.o assembler.o language.o Processor.o
	g++ -o mips64 pipeline.o utils.o simulator.o main.o assembler.o language.o Processor.o -lcurses

pipeline.o: pipeline.cpp utils.h mytypes.h

utils.o: utils.h utils.cpp mytypes.h

simulator.o: simulator.cpp simulator.h mytypes.h utils.h

main.o: main.cpp simulator.h mytypes.h utils.h

clean:
	rm -rf *.o mips64
