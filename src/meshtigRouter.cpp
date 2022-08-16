#include "meshtigRouter.hpp"
#include <cstdio>

void meshtigNode::info() {
    printf(
        "node<ip: %04X, score: %2f, visited: %i, bestScoreFrom: %04X>\n",
        this->ip, this->score, this->visited, this->bestScoreFrom
    );
}

void meshtigLink::info() {
    printf(
        "link<a: %04X, b: %04X, score: %2f>\n",
        this->a->ip, this->b->ip, this->score
    );
}

MeshtigRouter::MeshtigRouter() {
    this->ip = 0;
}

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

meshtigLink* MeshtigRouter::getLink(uint16_t _from, uint16_t _to) {
    uint16_t from  = std::min(_from, _to);
    uint16_t to    = std::max(_from, _to);
    
    for (int i = 0; i < this->links.size(); i++) {
        meshtigLink* link = this->links.at(i);
        if (link->a->ip == from && link->b->ip == to) {
            return link;
        }
    }
    return nullptr;
}

void MeshtigRouter::removeLink(uint16_t from, uint16_t to) {
    meshtigLink* link = getLink(from, to);
    for (int i = 0; i < link->a->links.size(); i++ ) {
        if (link->a->links.at(i) == link) {
            link->a->links.erase(link->a->links.begin() + i);
            break;
        }
    }
    for (int i = 0; i < link->b->links.size(); i++ ) {
        if (link->b->links.at(i) == link) {
            link->b->links.erase(link->b->links.begin() + i);
            break;
        }
    }
    for (int i = 0; i < this->links.size(); i++ ) {
        if (this->links.at(i) == link) {
            this->links.erase(this->links.begin() + i);
            break;
        }
    }
    delete link;
}

void MeshtigRouter::removeNode(uint16_t ip) {
    meshtigNode* node = getNode(ip);
    int nodeId = -1;
    for (int n = 0; n < this->nodes.size(); n++ ) {
        meshtigNode* cNode = this->nodes.at(n);
        if (cNode->ip == ip) {
            nodeId = n;
            continue;
        };
        for (int l = 0; l < cNode->links.size(); l++ ) {
            meshtigLink* cLink = cNode->links.at(l);
            if (cLink->a->ip == ip || cLink->b->ip == ip) {
                cNode->links.erase(cNode->links.begin() + l);

                // erase link from router vector
                for (int rl = 0; rl < this->links.size(); rl++) {
                    if (this->links.at(rl) == cLink) {
                        this->links.erase(this->links.begin() + rl);
                    }
                }
                delete cLink;
                break;
            }
        }
    }
    if (nodeId != -1)
        this->nodes.erase(this->nodes.begin() + nodeId);
    delete node;
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
    if (currentTarget != to) {
        currentTarget = to;
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

MeshtigRouter::~MeshtigRouter() {
    for (int i = 0; i < this->links.size(); i++) {
        delete this->links.at(i);
    }
    for (int i = 0; i < this->nodes.size(); i++) {
        delete this->nodes.at(i);
    }
}