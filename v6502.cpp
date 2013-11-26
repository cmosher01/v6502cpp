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
    AddressBus mem;
    mem.write(0x0200, 0xEA); // NOP
    mem.write(0x0201, 0xEA); // NOP
    mem.write(0x0202, 0xA9); // LDA #$5A
    mem.write(0x0203, 0x5A); //
    mem.write(0x0204, 0x85); // STA $88
    mem.write(0x0205, 0x88); //
    mem.write(0x0206, 0xA9); // LDA #$23
    mem.write(0x0207, 0x23); //
    mem.write(0x0208, 0xD0); // BNE 0
    mem.write(0x0209, 0xF8); //

    mem.write(0xFFFC, 0x02); // RESET --> $0202
    mem.write(0xFFFD, 0x02);

    CPU cpu(mem);


    /* Now let's just run some things to play with it. */
    std::cout << "begin power-up..." << std::endl;
    cpu.powerOn();

    std::cout << "some power-up pre-reset cycles..." << std::endl;
    for (int i(0); i < 10; ++i) {
        cpu.tick();
    }

    std::cout << "RESET..." << std::endl;
    cpu.reset();
    for (int i(0); i < 40; ++i) {
        cpu.tick();
    }

    return EXIT_SUCCESS;
}
