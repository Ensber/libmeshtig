#ifndef MESHTIG_STACK_HPP
#define MESHTIG_STACK_HPP

#include <cstdio>

#include "meshtigRouter.hpp"
#include "meshtigInterface.hpp"
#include "meshtigFrame.hpp"

typedef void (*onReceiveFnc)(MeshtigFrame::Frame* frame);

struct onReceiveFncProtos {
    MeshtigFrame::Protocol protocol;
    onReceiveFnc function;
};


class MeshtigStack {
private:
    void poll(uint16_t interfaceId);
public:
    std::vector<MeshtigInterface*> interfaces = std::vector<MeshtigInterface*>();
    std::vector<onReceiveFncProtos> onRecvs = std::vector<onReceiveFncProtos>();
    MeshtigRouter router = MeshtigRouter();
    
    std::vector<MeshtigFrame::Frame*> frameBuffer = std::vector<MeshtigFrame::Frame*>();
    size_t bufferSize = 8192; // bytes stored, before new packets get dropped TODO: maby the old ones?

    MeshtigStack();

    void step();

    void triggerPolling(); // poll all
    void triggerPolling(uint16_t interfaceIp);

    void addInterface(MeshtigInterface* interface);
    MeshtigInterface* getInterface(uint16_t ip);

    void send(MeshtigFrame::Frame* frame, bool fromBuffer);

    void registerProtoRecv(MeshtigFrame::Protocol protocol, onReceiveFnc onReceive);
    void unregisterProtoRecv(MeshtigFrame::Protocol protocol);

    ~MeshtigStack();
};

#endif