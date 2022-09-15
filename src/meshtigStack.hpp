#ifndef MESHTIG_STACK_HPP
#define MESHTIG_STACK_HPP

#include <cstdio>

#include "meshtigRouter.hpp"
#include "meshtigInterface.hpp"
#include "meshtigFrame.hpp"

typedef void (*onReceiveFnc)(void* userdata, MeshtigFrame::Frame* frame);

struct onReceiveFncProtos {
    MeshtigFrame::Protocol protocol;
    onReceiveFnc function;
    void* userdata;
};


class MeshtigStack {
private:
    std::vector<MeshtigFrame::Frame*> frameBuffer = std::vector<MeshtigFrame::Frame*>();
    size_t bufferSize = 0;
    uint16_t selfIp = 0;

    void poll(uint16_t interfaceId);
    void putFrameOnBuffer(MeshtigFrame::Frame* frame);
    void handleFrameInternal(MeshtigFrame::Frame* frame);
public:
    std::vector<MeshtigInterface*> interfaces = std::vector<MeshtigInterface*>();
    std::vector<onReceiveFncProtos> onRecvs = std::vector<onReceiveFncProtos>();
    MeshtigRouter router = MeshtigRouter();
    
    size_t maxBufferSize = 8192; // bytes stored, before new packets get dropped TODO: maby the old ones?

    MeshtigStack(uint16_t ip);

    void step();

    void triggerPolling(); // poll all
    void triggerPolling(uint16_t interfaceIp);

    void addInterface(MeshtigInterface* interface);
    MeshtigInterface* getInterface(uint16_t ip);

    bool send(MeshtigFrame::Frame* frame, bool fromBuffer);

    void registerProtoRecv(MeshtigFrame::Protocol protocol, onReceiveFnc onReceive, void* userdata);
    void unregisterProtoRecv(MeshtigFrame::Protocol protocol);

    ~MeshtigStack();
};

#endif