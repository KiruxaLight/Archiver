#include "writing.h"
#include <iostream>

Writer::Writer(std::ofstream& out) : out_(out), buffer_(0), cnt_(0) {
}

// Вывести
void Writer::Write(std::vector<bool> symbol) {
    // по 8 битов выводится (байт)
    for (const auto& i : symbol) {
        if (i) {
            buffer_ = (buffer_ | (1 << (8 - 1 - cnt_)));
            // 00110101 10111010 11100101 00001010 (34)
        }
        ++cnt_;
        if (static_cast<size_t>(cnt_) == 8) {
            out_.put(buffer_);

            cnt_ = 0;
            buffer_ = 0;
        }
    }
    // out_.flush();
}

void Writer::WriteNumber(size_t number, size_t len) {
    std::vector<bool> bit(len, false);
    size_t i = 0;
    while (number > 0) {
        bit[len - i - 1] = (number & 1);
        number >>= 1;
        ++i;
    }
    Write(bit);
}

void Writer::Clear() {
    if (cnt_ > 0) {
        out_.put(static_cast<char>(buffer_));
    }
    out_.flush();
}