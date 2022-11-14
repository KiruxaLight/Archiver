#pragma once

#include <fstream>
#include <vector>

const size_t BIT_COUNT = 9;

class Reader {
public:
    explicit Reader(std::ifstream& in);
    std::vector<bool> Read(size_t count);
    void Clear();
    bool IsEnd() const;

private:
    char buffer_;
    bool end_;
    size_t remaining_;
    std::ifstream& in_;
};
