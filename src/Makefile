CXXFLAGS = -Wall -ggdb

OBJS :=  pipeline.o utils.o simulator.o main.o assembler.o language.o Processor.o \
	CodeMemory.o DataMemory.o CPUConfig.o Terminal.o Screen.o PipelineHistory.o \
         MemoryRegion.o NullRegion.o IO.o CPUStats.o

all: mips64

mips64: $(OBJS)
	c++ -o mips64 $(OBJS) -lcurses

clean:
	rm -rf *.o mips64

# Gracias g++ -MM *.cpp
CPUConfig.o: CPUConfig.cpp CPUConfig.h mytypes.h
CPUStats.o: CPUStats.cpp mytypes.h CPUConfig.h pipeline.h CPUStats.h
CodeMemory.o: CodeMemory.cpp mytypes.h CodeMemory.h
DataMemory.o: DataMemory.cpp mytypes.h utils.h DataMemory.h Region.h \
  NullRegion.h MemoryRegion.h
IO.o: IO.cpp mytypes.h IO.h MemoryRegion.h Region.h Terminal.h Screen.h \
  DataMemory.h NullRegion.h
MemoryRegion.o: MemoryRegion.cpp mytypes.h utils.h MemoryRegion.h \
  Region.h
NullRegion.o: NullRegion.cpp mytypes.h utils.h NullRegion.h Region.h
PipelineHistory.o: PipelineHistory.cpp simulator.h mytypes.h utils.h \
  Processor.h CPUConfig.h pipeline.h DataMemory.h Region.h NullRegion.h \
  CodeMemory.h assembler.h IO.h MemoryRegion.h Terminal.h Screen.h \
  PipelineHistory.h CPUStats.h
Processor.o: Processor.cpp mytypes.h Processor.h CPUConfig.h pipeline.h \
  DataMemory.h Region.h NullRegion.h CodeMemory.h
Screen.o: Screen.cpp mytypes.h Screen.h
Terminal.o: Terminal.cpp Terminal.h mytypes.h
assembler.o: assembler.cpp simulator.h mytypes.h utils.h Processor.h \
  CPUConfig.h pipeline.h DataMemory.h Region.h NullRegion.h CodeMemory.h \
  assembler.h IO.h MemoryRegion.h Terminal.h Screen.h PipelineHistory.h \
  CPUStats.h language.h
language.o: language.cpp mytypes.h language.h
main.o: main.cpp simulator.h mytypes.h utils.h Processor.h CPUConfig.h \
  pipeline.h DataMemory.h Region.h NullRegion.h CodeMemory.h assembler.h \
  IO.h MemoryRegion.h Terminal.h Screen.h PipelineHistory.h CPUStats.h
pipeline.o: pipeline.cpp mytypes.h utils.h Processor.h CPUConfig.h \
  pipeline.h DataMemory.h Region.h NullRegion.h CodeMemory.h language.h \
  assembler.h
simulator.o: simulator.cpp mytypes.h utils.h Processor.h CPUConfig.h \
  pipeline.h DataMemory.h Region.h NullRegion.h CodeMemory.h simulator.h \
  assembler.h IO.h MemoryRegion.h Terminal.h Screen.h PipelineHistory.h \
  CPUStats.h
utils.o: utils.cpp mytypes.h utils.h
