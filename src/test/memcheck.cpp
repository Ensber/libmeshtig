#include <vector>


#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace std;

namespace memcheck {
    std::vector<size_t> memory = std::vector<size_t>();

    void pre_alloc() {

    }

    void post_free() {
        
    }
}


