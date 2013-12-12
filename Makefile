CXXFLAGS=-g -std=c++11

all: v6502

v6502: v6502.o cpu.o nodes.o trans.o SegmentCache.o TransNetwork.o
	g++ $^ -o $@

v6502.o: v6502.cpp cpu.h addressbus.h TransNetwork.h 

cpu.o: cpu.cpp cpu.h addressbus.h nodes.h

nodes.o: nodes.cpp nodes.h

trans.o: trans.cpp trans.h

SegmentCache.o: SegmentCache.cpp SegmentCache.h

TransNetwork.o: TransNetwork.cpp TransNetwork.h trans.h

clean:
	-rm *.o
	-rm v6502
	-rm v6502.exe
