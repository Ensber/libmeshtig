#ifndef MESHTIG_FRAME_HPP
#define MESHTIG_FRAME_HPP

#include <stdint.h>
#include <cstdio>

namespace MeshtigFrame {
    enum Protocol {
        PING,
        UDP,
        RAW,
        NTP,
        NETCTRL
    };

    struct Frame {
        uint16_t via;
        uint8_t  ttl = 255;
        Protocol protocol;
        uint16_t from;
        uint16_t to;
        uint32_t size;
        uint8_t* buffer;
        uint16_t seq;
    };
    const size_t HEADER_SIZE = 14;

    uint8_t* frameToBuffer(uint32_t &size, Frame* frame);
    Frame*   bufferToFrame(uint32_t size, uint8_t* buffer);
}

#endif