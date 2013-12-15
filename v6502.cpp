#include "addressbus.h"
#include "Cpu6502.h"
#include "Cpu6502Helper.h"
#include "TransNetwork.h"
#include "Trace.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <set>


int main(int argc, char *argv[]) {
    AddressBus mem;

    /* Load some test program into memory */
    mem.write(0x0200, 0xEA); // NOP
    mem.write(0x0201, 0xEA); // NOP
    mem.write(0x0202, 0xA9); // LDA #$5A
    mem.write(0x0203, 0x5A); //
    mem.write(0x0204, 0x85); // STA $88
    mem.write(0x0205, 0x88); //
    mem.write(0x0206, 0xA9); // LDA #$23
    mem.write(0x0207, 0x23); //
    mem.write(0x0208, 0xD0); // BNE $0200
    mem.write(0x0209, 0xF6); //

    mem.write(0xFFFC, 0x02); // RESET --> $0202
    mem.write(0xFFFD, 0x02);



    std::ifstream if_trans("transistors");
    if (!if_trans.is_open()) {
        std::cerr << "error opening file: transistors" << std::endl;
        exit(EXIT_FAILURE);
    }
    TransNetwork tn(if_trans);
    Trace trace(tn.segs);
    Cpu6502 cpu(tn, mem, trace);
    Cpu6502Helper cpuhelper(cpu);





    /* turn on the CPU */
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "begin power-up..." << std::endl;
    cpuhelper.powerOn();
    std::cout << "end power-up..." << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    /* run it a bit, before resetting */
    std::cout << "some power-up pre-reset cycles..." << std::endl;
    for (int i(0); i < 10; ++i) {
        cpuhelper.tick();
    }
    std::cout << "----------------------------------------" << std::endl;

    /* reset the CPU, and let it run for a little while, then exit */
    std::cout << "RESET..." << std::endl;
    cpuhelper.reset();
    for (int i(0); i < 50; ++i) {
        cpuhelper.tick();
    }

    return EXIT_SUCCESS;
}
