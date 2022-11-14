#pragma once

#include <istream>
#include <fstream>
#include <vector>

class Writer {
public:
    explicit Writer(std::ofstream& out);
    void Write(std::vector<bool> symbol);
    void WriteNumber(size_t number, size_t len);
    void Clear();

private:
    std::ofstream& out_;
    uint8_t buffer_;
    char cnt_;
};