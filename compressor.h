#pragma once
#include "writing.h"
#include <string>

class Compressor {
public:
    void Compress(const std::string& name, Writer& writer, bool end_archive);
private:
};