#ifndef MESHTIG_STACK_HPP
#define MESHTIG_STACK_HPP

#include "meshtigRouter.hpp"
#include "meshtigInterface.hpp"

class MeshtigStack {
public:
    std::vector<MeshtigInterface*> interfaces = std::vector<MeshtigInterface*>();
    MeshtigRouter router = MeshtigRouter();

    MeshtigStack();

    void step();

    void triggerPolling(); // poll all
    void triggerPolling(uint16_t interfaceIp);

    void addInterface(MeshtigInterface* interface);

    

    ~MeshtigStack();
};

#endif