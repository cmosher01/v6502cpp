#include <algorithm>
#include <utility>
#include <iterator>
#include <vector>
#include <set>
#include <map>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "addressbus.h"
#include "cpu.h"

#include "TransNetwork.h"

//memory[0xFF] = 0x68;  // PLA

//memory[0xFF] = 0xFF;
//memory[0xFFFF] = 0xFE;
//memory[0xFEFE] = 0xFD;
//memory[0xFDFD] = 0xFC;
//memory[0xFCFC] = 0xFB;
//memory[0xFBFB] = 0xFA;
//memory[0xFAFA] = 0xF9;

//void pZP() {
//    int a = 0;
//    for (int i = 0; i < 16; ++i) {
//        pHexw(a);
//        std::cout << ": ";
//        for (int j = 0; j < 16; ++j) {
//            pHex(memory[a++]);
//            std::cout << " ";
//        }
//        std::cout << std::endl;
//    }
//}

int main(int argc, char *argv[]) {
    std::ifstream if_trans("transistors");
    if (!if_trans.is_open()) {
        std::cerr << "error opening file: transistors" << std::endl;
        exit(EXIT_FAILURE);
    }
    TransNetwork tn(if_trans);
    SegmentCache::Common* n = tn.segs.c;
    n->CLK0->on = true;
}

int xxxmain(int argc, char *argv[]) {
    AddressBus mem;
    CPU cpu(mem);



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



    /* turn on the CPU */
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "begin power-up..." << std::endl;
    cpu.powerOn();
    std::cout << "end power-up..." << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    /* run it a bit, before resetting */
    std::cout << "some power-up pre-reset cycles..." << std::endl;
    for (int i(0); i < 10; ++i) {
        cpu.tick();
    }
    std::cout << "----------------------------------------" << std::endl;

    /* reset the CPU, and let it run for a little while, then exit */
    std::cout << "RESET..." << std::endl;
    cpu.reset();
    for (int i(0); i < 50; ++i) {
        cpu.tick();
    }

    return EXIT_SUCCESS;
}
