#pragma once

#include <vector>
#include <unordered_map>

class Canonical {
public:
    std::unordered_map<std::vector<bool>, std::vector<bool>> ToCanonical(
        std::vector<std::pair<size_t, std::vector<bool>>>& vec);

private:
};