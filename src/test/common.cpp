#include "common.hpp"

void assert(bool ok, std::string msg) {
    if (!ok) {
        printf(" #############################\n");
        printf(" # ERROR:\n");
        printf(" # %s", msg.c_str());
        exit(-1);
    }
}

void printAsHex(uint8_t* ptr, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", ptr[i]);
    }
    printf("\n");
}