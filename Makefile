CXXFLAGS=-g -std=c++11

all: v6502

v6502: v6502.o cpu.o nodes.o SegmentCache.o
	g++ $^ -o $@

v6502.o: v6502.cpp cpu.h addressbus.h

cpu.o: cpu.cpp cpu.h addressbus.h nodes.h

nodes.o: nodes.cpp nodes.h

SegmentCache.o : SegmentCache.cpp SegmentCache.h

clean:
	-rm *.o
	-rm v6502
