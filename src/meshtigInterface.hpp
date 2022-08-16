#ifndef MESHTIG_INTERFACE_HPP
#define MESHTIG_INTERFACE_HPP

#include <stdint.h>

struct MeshtigInterface {
    uint16_t ip;
    float speed;
    uint32_t maxPacketLength;

    // for transmission over serial or simmilar
    bool wired;

    uint16_t nChannels;
    uint16_t channel;

    // set channel
    void (*setCh)(MeshtigInterface* self, uint16_t ch);

    // size and buffer are filled and should be transmitted, on error return false
    bool (*send) (MeshtigInterface* self, uint32_t &size, char* buffer);

    // returns a pointer to a charbuffer, or 0 if nothing was received
    char*(*recv) (MeshtigInterface* self, uint32_t &size);

    void* userdata = nullptr;
};

#endif