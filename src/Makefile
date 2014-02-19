CXXFLAGS = -Wall

all: mips64

mips64: pipeline.o utils.o processor.o main.o
	g++ -o mips64 pipeline.o utils.o processor.o main.o -lcurses

pipeline.o: pipeline.cpp utils.h mytypes.h

utils.o: utils.h utils.cpp mytypes.h

processor.o: processor.cpp processor.h mytypes.h utils.h

main.o: main.cpp processor.h mytypes.h utils.h

clean:
	rm -rf *.o mips64
