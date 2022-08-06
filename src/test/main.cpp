#include "meshtig/meshtigRouter.hpp"
#include <cstdio>

/*
R1--R2
| \ |
R3--R4
|
R5
*/

int main() {

    MeshtigRouter R0 = MeshtigRouter(0x1010);
    R0.addNode(0x1020);
    R0.addNode(0x1030);
    R0.addNode(0x1040);
    R0.addNode(0x1050);

    R0.addLink(0x1010, 0x1020, 1);
    R0.addLink(0x1010, 0x1030, 1);
    R0.addLink(0x1010, 0x1040, 1);
    R0.addLink(0x1020, 0x1040, 1);
    R0.addLink(0x1030, 0x1040, 1);
    R0.addLink(0x1030, 0x1050, 1);

    uint16_t from = 0x1020;
    uint16_t to   = 0x1050;
    routingResponse res;

    while (true) {
        printf("%04X -> %04X\n", from, to);
        routingResponse res = R0.route(from, to);
        if (res.error == routingResponseErrors::TARGET_EQ_SOURCE) break;
        if (res.error) {
            printf("ERROR: %i\n", res.error);
            break;
        }
        // printf("%04X -> %04X\n", from, to);
        from = res.next;
    }

    return 0;
}