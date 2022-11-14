#include "duty.h"

bool GetBit(size_t mask, size_t index) {
    return (mask >> index & 1);
}

size_t ToDec(const std::vector<bool> &a) {
    size_t res = 0;
    for (auto i : a) {
        res <<= 1;
        res += i;
    }
    return res;
}