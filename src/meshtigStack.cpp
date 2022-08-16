#include "meshtigStack.hpp"

// private //

void MeshtigStack::poll(uint16_t interfaceId) {

}

// public //

MeshtigStack::MeshtigStack() {

}

void MeshtigStack::step() {

}

void MeshtigStack::triggerPolling() {
    
}

void MeshtigStack::triggerPolling(uint16_t interfaceIp) {
    
}

void MeshtigStack::addInterface(MeshtigInterface* interface) {
    this->interfaces.push_back(interface);
}

MeshtigInterface* MeshtigStack::getInterface(uint16_t ip) {
    MeshtigInterface* cInt;
    for (int i = 0; i < this->interfaces.size(); i++) {
        cInt = this->interfaces.at(i);
        if (cInt->ip == ip) {
            return cInt;
        }
    }
    return nullptr;
}

void MeshtigStack::send(MeshtigFrame::Frame* frame, bool fromBuffer = false) {
    // route the via of the frame to the next ip,
    // that is not controlled by the stack or is the
    // target ip
    frame->via = frame->from;
    uint16_t emitterInterfaceIp;
    while (true) {
        emitterInterfaceIp = frame->via;
        auto res = this->router.route(frame->via, frame->to);
        if (res.error == routingResponseErrors::TARGET_EQ_SOURCE) break;
        if (res.error) {
            printf("FATAL ERROR: UNROUTABLE PACKET! (<routingResponseErrors> code: %i)", res.error);
            exit(-1);
        }
        if (!this->getInterface(res.next)) break;
    }


}

// allows definition of multiple receiver functions per protocol, if needed
void MeshtigStack::registerProtoRecv(MeshtigFrame::Protocol protocol, onReceiveFnc onReceive) {
    onReceiveFncProtos temp;
    temp.protocol = protocol;
    temp.function = onReceive;
    this->onRecvs.push_back(temp);
}

// removes all functons associated with one protocol
void MeshtigStack::unregisterProtoRecv(MeshtigFrame::Protocol protocol) {
    for (int i = this->onRecvs.size() - 1; i != -1; i++) {
        if (this->onRecvs.at(i).protocol == protocol) {
            this->onRecvs.erase(this->onRecvs.begin() + i);
        }
    }
}

MeshtigStack::~MeshtigStack() {
    for (int i = 0; i < this->interfaces.size(); i++) {
        MeshtigInterface* cInt = this->interfaces.at(i);
        delete cInt->userdata;
        delete cInt;
    }
}