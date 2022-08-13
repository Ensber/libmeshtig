#include "meshtig/meshtigRouter.hpp"
#include <cstdio>
#include "memcheck.cpp"

/*
R1--R2
| \ |
R3--R4
|
R5
*/

MeshtigRouter* getDefault() {
    MeshtigRouter* R0 = new MeshtigRouter(0x1010);
    R0->addNode(0x1020);
    R0->addNode(0x1030);
    R0->addNode(0x1040);
    R0->addNode(0x1050);

    R0->addLink(0x1010, 0x1020, 1);
    R0->addLink(0x1010, 0x1030, 1);
    R0->addLink(0x1010, 0x1040, 1);
    R0->addLink(0x1020, 0x1040, 1);
    R0->addLink(0x1030, 0x1040, 1);
    R0->addLink(0x1030, 0x1050, 1);
    return R0;
}

bool routeFromTo(MeshtigRouter* R0, uint16_t from, uint16_t to, bool cleanupRouter = true) {
    routingResponse res;

    while (true) {
        printf("%04X -> %04X\n", from, to);
        routingResponse res = R0->route(from, to);
        if (res.error == routingResponseErrors::TARGET_EQ_SOURCE) break;
        if (res.error) {
            printf("ERROR: %i\n", res.error);

            // print netork values
            printf("Nodes:\n");
            for (int i = 0; i < R0->nodes.size(); i++) {
                meshtigNode* cNode = R0->nodes.at(i);
                cNode->info();
                for (int j = 0; j < cNode->links.size(); j++) {
                    meshtigLink* cLink = cNode->links.at(j);
                    printf(
                        "  -> link<ip: %04X, score: %2f>\n",
                        (cLink->a == cNode ? cLink->b : cLink->a)->ip, cLink->score
                    );
                    
                }
            }
            printf("\nLinks:\n");
            for (int i = 0; i < R0->links.size(); i++) {
                meshtigLink* cLink = R0->links.at(i);
                cLink->info();
            }

            delete R0;
            return false;
        }
        from = res.next;
    }

    delete R0;
    return true;
}

void assert(bool ok, std::string msg) {
    if (!ok) {
        printf("%s", msg.c_str());
        exit(-1);
    }
}

int main() {

    // try to remove routes
    printf("\n == Testing Route removal\n");
    MeshtigRouter* R0 = getDefault();
    R0->removeLink(0x1010, 0x1030);
    R0->removeLink(0x1020, 0x1040);
    assert(routeFromTo(R0, 0x1020, 0x1050), "Route removal failed");

    // try to remove routes
    printf("\n == Testing Node removal\n");
    R0 = getDefault();
    R0->removeNode(0x1040);
    assert(routeFromTo(R0, 0x1020, 0x1050), "Node removal failed");

    return 0;
}