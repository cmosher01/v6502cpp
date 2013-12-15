CXXFLAGS=-g -std=c++11 -Wall -O0

SRCS = v6502.cpp SegmentCache.cpp Common.cpp TransNetwork.cpp Trace.cpp Circuit.cpp StateCalculator.cpp Cpu6502.cpp Cpu6502Helper.cpp
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
