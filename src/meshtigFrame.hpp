#ifndef MESHTIG_FRAME_HPP
#define MESHTIG_FRAME_HPP

#include <stdint.h>

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
        uint8_t  ttl;
        Protocol protocol;
        uint16_t from;
        uint16_t to;
        uint32_t size;
        uint8_t* buffer;
    };

    uint8_t*  frameToBuffer(uint32_t &size, Frame* frame);
    Frame* bufferToFrame(uint32_t size, uint8_t* buffer);
}

#endif