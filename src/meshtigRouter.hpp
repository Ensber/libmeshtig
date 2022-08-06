#include <stdint.h>
#include <vector>
#include <math.h>

struct meshtigLink;

enum visitedStates {
    OPEN,
    PENDING,
    VISITED
};

struct meshtigNode {
    uint16_t ip;
    visitedStates visited;
    float score;
    uint16_t bestScoreFrom;
    std::vector<meshtigLink*> links = std::vector<meshtigLink*>();
};

struct meshtigLink {
    meshtigNode* a;
    meshtigNode* b;
    float score;
};

enum routingResponseErrors {
    OK,
    SOURCE_NOT_FOUND,
    TARGET_NOT_FOUND,
    TARGET_NOT_REACHED,
    TARGET_EQ_SOURCE
};

struct routingResponse
{
    uint16_t next;
    routingResponseErrors error;
};



class MeshtigRouter {
private:
    uint16_t currentTarget;
public:
    std::vector<meshtigNode*> nodes = std::vector<meshtigNode*>();
    std::vector<meshtigLink*> links = std::vector<meshtigLink*>();
    uint16_t ip;

    MeshtigRouter(uint16_t ip);

    void addNode(uint16_t ip);
    meshtigNode* getNode(uint16_t ip);
    void addLink(uint16_t from, uint16_t to, float score);

    routingResponse route(uint16_t from, uint16_t to);
    routingResponse route(uint16_t to);
};