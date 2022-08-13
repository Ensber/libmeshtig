#ifndef MESHTIG_ROUTER_HPP
#define MESHTIG_ROUTER_HPP

#include <stdint.h>
#include <vector>
#include <math.h>

struct meshtigLink;

enum visitedStates {
    OPEN,
    PENDING,
    VISITED
};

class meshtigNode {
public:
    uint16_t ip;
    visitedStates visited;
    float score;
    uint16_t bestScoreFrom;
    std::vector<meshtigLink*> links = std::vector<meshtigLink*>();
    void info();
};

class meshtigLink {
public:
    meshtigNode* a;
    meshtigNode* b;
    float score;
    void info();
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

    MeshtigRouter();
    MeshtigRouter(uint16_t ip);

    void addNode(uint16_t ip);
    meshtigNode* getNode(uint16_t ip);
    void addLink(uint16_t from, uint16_t to, float score);
    meshtigLink* getLink(uint16_t from, uint16_t to);

    void removeNode(uint16_t ip);
    void removeLink(uint16_t from, uint16_t to);

    routingResponse route(uint16_t from, uint16_t to);
    routingResponse route(uint16_t to);

    ~MeshtigRouter();
};

#endif