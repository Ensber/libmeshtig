#include "meshtigRouter.hpp"
#include <cstdio>

MeshtigRouter::MeshtigRouter(uint16_t ip) {
    this->ip = ip;
    this->addNode(ip);
}

void MeshtigRouter::addNode(uint16_t ip) {
    meshtigNode* node = new meshtigNode();
    node->ip = ip;
    this->nodes.push_back(node);
}

meshtigNode* MeshtigRouter::getNode(uint16_t ip) {
    for (int i = 0; i < this->nodes.size(); i++) {
        if (this->nodes.at(i)->ip == ip) {
            return this->nodes.at(i);
        }
    }
    printf("Could not find a node with ip %04X\n", ip);
    throw;
    return nullptr;
}

void MeshtigRouter::addLink(uint16_t _from, uint16_t _to, float score) {
    meshtigLink* link = new meshtigLink();
    uint16_t from  = std::min(_from, _to);
    uint16_t to    = std::max(_from, _to);
    meshtigNode* a = this->getNode(from);
    meshtigNode* b = this->getNode(to);
    link->a = a;
    link->b = b;
    link->score = score;
    this->links.push_back(link);
    a->links.push_back(link);
    b->links.push_back(link);
}

routingResponse MeshtigRouter::route(uint16_t from, uint16_t to) {
    routingResponse res;

    if (from == to) {
        res.error = routingResponseErrors::TARGET_EQ_SOURCE;
        return res;
    }

    // setup routing
    meshtigNode* fromNode = this->getNode(from);
    meshtigNode* toNode   = this->getNode(to);

    if (!fromNode) {
        res.error = routingResponseErrors::SOURCE_NOT_FOUND;
        return res;
    }
    if (!toNode) {
        res.error = routingResponseErrors::TARGET_NOT_FOUND;
        return res;
    }

    for (int i = 0; i < this->nodes.size(); i++) {
        meshtigNode* cNode = this->nodes.at(i);
        cNode->bestScoreFrom = 0;
        cNode->score = INFINITY;
    }
    toNode->score = 0;
    toNode->visited = visitedStates::PENDING;

    // do routing
    while (true) {
        int nPending = 0;
        for (int i = 0; i < this->nodes.size(); i++) {
            meshtigNode* cNode = this->nodes.at(i);
            if (cNode->visited == visitedStates::PENDING) {
                cNode->visited = visitedStates::VISITED;
                for (int j = 0; j < cNode->links.size(); j++) {
                    meshtigLink* cLink = cNode->links.at(j);
                    meshtigNode* otherNode = (cLink->a != cNode) ? cLink->a : cLink->b;
                    float linkScore = cNode->score + cLink->score;
                    if (otherNode->score > linkScore) {
                        otherNode->score = linkScore;
                        otherNode->bestScoreFrom = cNode->ip;
                        otherNode->visited = visitedStates::PENDING;
                        nPending+=1;
                    }
                }
            }
        }
        if (!nPending) break;
    }
    
    if (fromNode->bestScoreFrom) {
        res.error = routingResponseErrors::OK;
        res.next  = fromNode->bestScoreFrom;
    } else {
        res.error = routingResponseErrors::TARGET_NOT_FOUND;
    }

    return res;
}

// schienenersatzverkehrsfunktion
routingResponse MeshtigRouter::route(uint16_t to) {
    return this->route(this->ip, to);
}