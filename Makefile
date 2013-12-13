CXXFLAGS=-g -std=c++11

SRCS = v6502.cpp cpu.cpp nodes.cpp trans.cpp SegmentCache.cpp TransNetwork.cpp Trace.cpp Circuit.cpp StateCalculator.cpp Cpu6502.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

.cpp.o:
	$(COMPILE.cpp) -MMD $(OUTPUT_OPTION) $<

.PHONY: all clean



all: v6502

v6502: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) -o $@

-include $(DEPS)



clean:
	-rm -f v6502 v6502.exe $(OBJS) $(DEPS)
