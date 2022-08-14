#include "meshtigFrame.hpp"
#include <string.h>
#include <cstdio>

uint8_t* MeshtigFrame::frameToBuffer(uint32_t &size, Frame* frame) {
    size = 12 + frame->size;
    uint8_t* buffer = new uint8_t[size];

    buffer[0]  = frame->via >> 8;
    buffer[1]  = frame->via & 0xFF;
    buffer[2]  = frame->ttl;
    buffer[3]  = frame->protocol;
    buffer[4]  = frame->from >> 8;
    buffer[5]  = frame->from & 0xFF;
    buffer[6]  = frame->to >> 8;
    buffer[7]  = frame->to & 0xFF;
    buffer[8]  = frame->size >> 24       ;
    buffer[9]  = frame->size >> 16 & 0xFF;
    buffer[10] = frame->size >> 8  & 0xFF;
    buffer[11] = frame->size       & 0xFF;

    memcpy(buffer + 12, frame->buffer, frame->size);

    return buffer;
}

MeshtigFrame::Frame* MeshtigFrame::bufferToFrame(uint32_t size, uint8_t* buffer) {
    Frame* frame = new Frame;

    frame->via = (buffer[0] << 8) + buffer[1];
    frame->ttl = buffer[2];
    frame->protocol = (Protocol)buffer[3];
    frame->from = (buffer[4] << 8) + buffer[5];
    frame->to   = (buffer[6] << 8) + buffer[7];
    frame->size = (buffer[8] << 24) + (buffer[9] << 16) + (buffer[10] << 8) + buffer[11];
    
    frame->buffer = new uint8_t[frame->size];
    memcpy(frame->buffer, buffer + 12, frame->size);

    return frame;
}