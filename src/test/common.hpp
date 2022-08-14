#ifndef COMMON_HPP
#define COMMON_HPP

#include <cstdio>
#include <string>

void assert(bool ok, std::string msg);
void printAsHex(uint8_t* ptr, size_t size);

#endif