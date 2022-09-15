#include "test-frame.hpp"
#include "common.hpp"
#include <string.h>

void testFrame() {
    printf("\n==== FRAME TEST ====");
    printf("\n == encode/decode\n");

    MeshtigFrame::Frame frame = MeshtigFrame::Frame();
    frame.from = 0x1010;
    frame.to   = 0x1050;
    frame.via  = 0x1020;
    frame.ttl  = 255;
    frame.protocol = MeshtigFrame::Protocol::RAW;

    frame.size = 12;
    std::string hello_world = "Hello World";
    frame.buffer = (uint8_t*)hello_world.c_str();

    uint32_t size;
    uint8_t* buffer = MeshtigFrame::frameToBuffer(size, &frame);
    printf("Encoded frame: ");
    printAsHex(buffer, size);
    printf("Valid frame  : 10 20 FF 02 10 10 10 50 00 00 00 0C 00 00 48 65 6C 6C 6F 20 57 6F 72 6C 64 00 \n");
    printf("Explaination : |VIA|TTL|PR|FROM |TO   |SIZE       |SEQ  |PAYLOAD                            |\n");
    
    /*
        1020     VIA
        AA       TTL (170)
        02       PROTO
        1010     FROM
        1050     TO
        0000000C SIZE (12)
        48656C6C6F20576F726C6400
    */

    MeshtigFrame::Frame* reconstructedFrame = MeshtigFrame::bufferToFrame(size, buffer);
    buffer = MeshtigFrame::frameToBuffer(size, reconstructedFrame);
    printf("Encoded frame: ");
    printAsHex(buffer, size);
    
    assert(frame.from     == reconstructedFrame->from    , "Frame field 'from' missmatch\n");
    assert(frame.to       == reconstructedFrame->to      , "Frame field 'to' missmatch\n");
    assert(frame.via      == reconstructedFrame->via     , "Frame field 'via' missmatch\n");
    assert(frame.size     == reconstructedFrame->size    , "Frame field 'size' missmatch\n");
    assert(frame.ttl      == reconstructedFrame->ttl     , "Frame field 'ttl' missmatch\n");
    assert(frame.protocol == reconstructedFrame->protocol, "Frame field 'protocol' missmatch\n");
    assert(frame.seq      == reconstructedFrame->seq     , "Frame field 'seq' missmatch\n");
    
    printf("Body original     : ");
    printAsHex(frame.buffer, frame.size);
    printf("Body reconstructed: ");
    printAsHex(reconstructedFrame->buffer, frame.size);
    assert(memcmp(frame.buffer, reconstructedFrame->buffer, frame.size) == 0, "Body reconstruction failed\n");

    // same as valid frame. in case of a unexpectet change in the frame, this
    // will catch it 
    uint8_t validFrame[] = {0x10, 0x20, 0xFF, 0x02, 0x10, 0x10, 0x10, 0x50, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x00};
    bool valid = size == sizeof(validFrame);
    if (valid) {
        for (int i = 0; i < frame.size; i++) {
            if (buffer[i] != validFrame[i]) {
                valid = false;
                printf("%i\n", i);
                break;
            }
        }
    }
    assert(valid, "Valid frame does not match. Did the frame format change?\n");
}