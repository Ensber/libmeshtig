#include "meshtigStack.hpp"
#include "interfaces/int-void.hpp"

// private //

// poll/receive from a interface
void MeshtigStack::poll(uint16_t interfaceId) {
    MeshtigInterface* interface = this->interfaces.at(interfaceId);
    uint32_t size = 0;
    uint8_t* buffer = interface->recv(interface, size);
    if (!size) return; // no new data
    MeshtigFrame::Frame* frame = MeshtigFrame::bufferToFrame(size, buffer);
    delete[] buffer;
    
    // check if the target is in this stack
    MeshtigInterface* target = this->getInterface(frame->to);
    if (target) {
        // handle the frame
        this->handleFrameInternal(frame);
    } else {
        // send the frame to the next hop
        this->send(frame, false);
    }
}

// puts frame on buffer, as long as there is space
void MeshtigStack::putFrameOnBuffer(MeshtigFrame::Frame* frame) {
    size_t newBufferSize = this->bufferSize + MeshtigFrame::HEADER_SIZE + frame->size;
    if (newBufferSize > this->maxBufferSize) {
        delete[] frame->buffer;
        delete frame;
    } else {
        this->frameBuffer.push_back(frame);
    }
}

// forward the frame to the handlers with the same protocol id
void MeshtigStack::handleFrameInternal(MeshtigFrame::Frame* frame) {
    for (int i = 0; i < this->onRecvs.size(); i++) {
        onReceiveFncProtos cRecvs = this->onRecvs.at(i);
        if (cRecvs.protocol == frame->protocol) {
            cRecvs.function(cRecvs.userdata, frame);
        }
    }
}

// public //

MeshtigStack::MeshtigStack(uint16_t ip) {
    this->selfIp = ip;
    this->addInterface(newVoidInterface(ip));
}

void MeshtigStack::step() {

}

void MeshtigStack::triggerPolling() {
    for (int i = 0; i < this->interfaces.size(); i++) {
        this->poll(i);
    }
}

void MeshtigStack::triggerPolling(uint16_t interfaceIp) {
    for (int i = 0; i < this->interfaces.size(); i++) {
        if (this->interfaces.at(i)->ip == interfaceIp) {
            this->poll(i);
            break;
        }
    }
}

void MeshtigStack::addInterface(MeshtigInterface* interface) {
    this->interfaces.push_back(interface);
    this->router.addNode(interface->ip);
    if (interface->ip != this->selfIp) {
        this->router.addLink(interface->ip, this->selfIp, 1);
    }
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

// returns, if the packet hast been transmitted/entered the sack
// deletes frame on transmission. alternatively puts it in the buffer or deletes it
bool MeshtigStack::send(MeshtigFrame::Frame* frame, bool fromBuffer = false) {
    // route the via of the frame to the next ip,
    // that is not controlled by the stack or is the
    // target ip
    printf("initial from ip 0x%04X\n", frame->from);
    printf("initial via  ip 0x%04X\n", frame->via);
    frame->via = frame->from;
    uint16_t emitterInterfaceIp = frame->via;
    for (;;) {
        uint16_t lastVia = frame->via;
        printf("[loop a] 0x%04X\n", frame->via);
        printf("[loop b] 0x%04X\n", lastVia);
        auto res = this->router.route(frame->via, frame->to);
        if (res.error == routingResponseErrors::TARGET_EQ_SOURCE) {
            this->handleFrameInternal(frame);
            delete[] frame->buffer;
            delete frame;
            return true;
        }
        if (res.error) {
            // todo
            printf("ERROR: UNROUTABLE PACKET! (<routingResponseErrors> code: %i)", res.error);
            // exit(-1);
            return false;
        }
        frame->via = res.next;
        printf("[loop c next] 0x%04X\n", frame->via);
        emitterInterfaceIp = lastVia;
        printf("Kandosii!\n");
        
        // break, when the next interface is no more available on this router
        // triggers frame transmission further down in the function
        if (!this->getInterface(res.next)) break;
    }
    
    // try to send the message via the emitter interface
    MeshtigInterface* emitter = this->getInterface(emitterInterfaceIp);
    if (!emitter) {
        // this should practically never occour...
        printf("WTF? %04X\n", emitterInterfaceIp);
        return false;
    } else {
        // try to send the frame
        uint32_t size;
        auto x = frame->buffer;
        uint8_t* buffer = MeshtigFrame::frameToBuffer(size, frame);
        bool sendSuccess = emitter->send(emitter, size, buffer);
        // cleanup
        delete[] buffer;
        if (sendSuccess) {
            delete[] frame->buffer;
            delete frame;
            return true;
        }
    }


    // if not on buffer, put on buffer
    // if the buffer is full, the frame gets dropped
    if (!fromBuffer) {
        this->putFrameOnBuffer(frame);
        return true;
    }
    return false;
}

// allows definition of multiple receiver functions per protocol, if needed
void MeshtigStack::registerProtoRecv(MeshtigFrame::Protocol protocol, onReceiveFnc onReceive, void* userdata) {
    onReceiveFncProtos temp;
    temp.protocol = protocol;
    temp.function = onReceive;
    temp.userdata = userdata;
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
        delete cInt;
    }
}