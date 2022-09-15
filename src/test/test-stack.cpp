#include "test-stack.hpp"
#include "interfaces/int-void.hpp"

// adds a stack with multiple nodes, aka interfaces, wich link together
MeshtigStack* defaultStack() {
    MeshtigStack* stack = new MeshtigStack(0x1010);
    stack->addInterface(newVoidInterface(0x1011));
    stack->addInterface(newVoidInterface(0x1012));
    stack->addInterface(newVoidInterface(0x1013));
    return stack;
}

void testStack() {
    printf("\n==== STACK TEST ====\n");
    printf(" == building default stack\n");
    MeshtigStack* S0 = defaultStack();
    printf(" == testing self send\n");
    ProtocolRaw raw = ProtocolRaw(S0, 0x1011);
    raw.send("-hello stack-", 14, 0x1013);
}