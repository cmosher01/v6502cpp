#ifndef ADDRESSBUS_H
#define ADDRESSBUS_H

class AddressBus {
private:
    unsigned char memory[0x10000];

public:

    AddressBus() {
        for (int i = 0; i < 0x10000; ++i) {
            memory[i] = 0x95;
        }
    }
    ~AddressBus() {}

    unsigned char read(unsigned short addr) {
        return memory[addr];
    }

    void write(unsigned short addr, unsigned char data) {
        memory[addr] = data;
    }
};

#endif
