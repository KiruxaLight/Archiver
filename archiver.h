#pragma once

#include "writing.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <map>
#include <set>
#include <bitset>
#include <vector>

const std::vector<bool> FILENAME_END = {true, false, false, false, false, false, false, false, false};
const std::vector<bool> ONE_MORE_FILE = {true, false, false, false, false, false, false, false, true};
const std::vector<bool> ARCHIVE_END = {true, false, false, false, false, false, false, true, false};

class Archiver {
public:
    void Compress(const std::string& name, Writer& writer, bool end_archive);
    void Decompress(const std::string& name);
    void OutHelp();
};