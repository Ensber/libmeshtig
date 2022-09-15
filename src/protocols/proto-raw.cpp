#include "proto-raw.hpp"

int ProtocolRaw::idCounter = 0;

void rawOnRecv(void* userdata, MeshtigFrame::Frame* frame) {
    ProtocolRaw* self = (ProtocolRaw*)userdata;
    // for (int i = 0; i < frame->size; i++) printf("[%2i] byte: %2X - %c - %3i\n", i, frame->buffer[i], frame->buffer[i], frame->buffer[i]);
    printf("%s:%i received \"%*s\" from %4X to %4X\n", self->name.c_str(), self->id, frame->size - 1, frame->buffer, frame->from, frame->to);
}

ProtocolRaw::ProtocolRaw(MeshtigStack* stack, uint16_t ip) {
    this->idCounter++;
    this->id = this->idCounter;
    this->stack = stack;
    this->ip = ip;

    // register onreceive in stack
    this->stack->registerProtoRecv(MeshtigFrame::Protocol::RAW, &rawOnRecv, this);
}

void ProtocolRaw::send(char* text, uint32_t length, uint16_t to) {
    MeshtigFrame::Frame* frame = new MeshtigFrame::Frame;
    frame->buffer = new uint8_t[length];
    memcpy(frame->buffer, text, length);
    frame->from = this->ip;
    frame->to = to;
    frame->via = frame->from;
    frame->protocol = MeshtigFrame::Protocol::RAW;
    frame->size = length;
    this->stack->send(frame, false);
}

ProtocolRaw::~ProtocolRaw() {}