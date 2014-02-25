CXXFLAGS = -Wall -ggdb

OBJS :=  pipeline.o utils.o simulator.o main.o assembler.o language.o Processor.o \
	CodeMemory.o DataMemory.o CPUConfig.o Terminal.o Screen.o

all: mips64

mips64: $(OBJS)
	c++ -o mips64 $(OBJS) -lcurses

clean:
	rm -rf *.o mips64

# Gracias g++ -MM *.cpp
CodeMemory.o: CodeMemory.cpp mytypes.h CodeMemory.h
DataMemory.o: DataMemory.cpp mytypes.h utils.h DataMemory.h
Processor.o: Processor.cpp mytypes.h Processor.h CPUConfig.h pipeline.h \
 DataMemory.h CodeMemory.h
Screen.o: Screen.cpp mytypes.h Screen.h
Terminal.o: Terminal.cpp Terminal.h mytypes.h
assembler.o: assembler.cpp simulator.h mytypes.h utils.h Processor.h \
 CPUConfig.h pipeline.h DataMemory.h CodeMemory.h Screen.h assembler.h \
 Terminal.h language.h
language.o: language.cpp mytypes.h language.h
main.o: main.cpp simulator.h mytypes.h utils.h Processor.h CPUConfig.h \
 pipeline.h DataMemory.h CodeMemory.h Screen.h assembler.h Terminal.h
pipeline.o: pipeline.cpp mytypes.h utils.h Processor.h CPUConfig.h \
 pipeline.h DataMemory.h CodeMemory.h language.h assembler.h
simulator.o: simulator.cpp mytypes.h utils.h Processor.h CPUConfig.h \
 pipeline.h DataMemory.h CodeMemory.h simulator.h Screen.h assembler.h \
 Terminal.h
utils.o: utils.cpp mytypes.h utils.h
