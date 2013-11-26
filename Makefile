all: v6502

v6502: v6502.o cpu.o
	g++ $^ -o $@

v6502.o: v6502.cpp cpu.h addressbus.h

cpu.o: cpu.cpp cpu.h addressbus.h nodes.h

clean:
	rm *.o
