#include "reading.h"
#include "duty.h"
#include <iostream>

Reader::Reader(std::ifstream& in) : buffer_(0), end_(false), remaining_(0), in_(in) {
}

std::vector<bool> Reader::Read(size_t count) {
    std::vector<bool> result;
    for (size_t i = 0; i < count; ++i) {
        if (remaining_ == 0) {
            buffer_ = 0;
            remaining_ = 8;
            in_.get(reinterpret_cast<char&>(buffer_));
            if (in_.eof()) {
                buffer_ = 0;
                remaining_ = 8;
                end_ = true;
            }
        }
        result.push_back(GetBit(buffer_, remaining_ - 1));
        --remaining_;
    }
    return result;
}

void Reader::Clear() {
    buffer_ = 0;
    end_ = false;
    remaining_ = 0;
    in_.clear();
    in_.seekg(0);
}

bool Reader::IsEnd() const {
    return end_;
}
