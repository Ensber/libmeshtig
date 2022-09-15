#ifndef PROTO_RAW_HPP
#define PROTO_RAW_HPP

#include <string>
#include <string.h>
#include "../meshtigStack.hpp"

class ProtocolRaw {
private:
    static int idCounter;
public:
    int id;
    MeshtigStack* stack;
    uint16_t ip;

    ProtocolRaw(MeshtigStack* stack, uint16_t selfIp);

    std::string name = "<rawHandler>";

    void send(char* text, uint32_t length, uint16_t to);

    ~ProtocolRaw();
};

#endif