#include "int-void.hpp"

void setCh(MeshtigInterface* self, uint16_t ch) {}

bool send(MeshtigInterface* self, uint32_t &size, uint8_t* buffer) {
    return true; // yes, we definetly just send the packet... into the void
}

uint8_t* recv(MeshtigInterface* self, uint32_t &size) {
    return nullptr;
}

struct MeshtigInterface* newVoidInterface(uint16_t ip) {
    struct MeshtigInterface* intf = new struct MeshtigInterface();

    intf->ip = ip;
    intf->channel = 0;
    intf->nChannels = 1;
    intf->speed = 1;
    intf->maxPacketLength = 100;
    intf->wired = false;

    intf->send = &send;
    intf->setCh = &setCh;
    intf->recv = &recv;

    return intf;
}