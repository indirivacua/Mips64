CXXFLAGS = -Wall

all: mips64

mips64: pipeline.o utils.o processor.o main.o
	g++ -o mips64 pipeline.o utils.o processor.o main.o -lcurses

clean:
	rm -rf *.o mips64
