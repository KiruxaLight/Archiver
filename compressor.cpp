#include "compressor.h"
#include "reading.h"
#include "huffman.h"
#include "canonical.h"
#include "duty.h"
#include "archiver.h"
#include <memory>

void Compressor::Compress(const std::string& path, Writer& writer, bool end_archive) {
    size_t slash_index = path.rfind('/');
    if (slash_index == std::string::npos) {
        slash_index = -1;
    }
    std::string name = path.substr(slash_index + 1);
    std::cout << name << "\n";
    std::ifstream in(path, std::ios::binary);
    std::vector<std::vector<bool>> elements;
    std::unordered_map<std::vector<bool>, size_t> frequency;
    frequency[FILENAME_END] = 1;
    frequency[ONE_MORE_FILE] = 1;
    frequency[ARCHIVE_END] = 1;

    for (auto x : name) {
        std::vector<bool> who(BIT_COUNT);
        for (size_t i = 0; i < BIT_COUNT; ++i) {
            who[BIT_COUNT - 1 - i] = GetBit(x, i);
        }
        elements.push_back(who);
        ++frequency[who];
    }

    Reader reader(in);
    size_t cnt = 0;
    while (true) {
        std::vector<bool> symbol = reader.Read(8);
        if (reader.IsEnd()) {
            break;
        }
//        std::cout << cnt << " : ";
//        for (auto i : symbol) {
//            std::cout << i;
//        }
//        std::cout << "\n";
        ++cnt;
        symbol.insert(symbol.begin(), false);  // 9 bit надо
        ++frequency[symbol];
    }

    reader.Clear();

    std::unique_ptr<Node> huffman(Node::Build(frequency));
    std::unordered_map<std::vector<bool>, size_t> table = huffman->BFS();
    std::vector<std::pair<size_t, std::vector<bool>>> vec;
    for (auto& [symbol, length] : table) {
        vec.push_back({length, symbol});
    }
    std::sort(vec.begin(), vec.end());
    Canonical canonical;
    std::unordered_map<std::vector<bool>, std::vector<bool>> canonical_codes = canonical.ToCanonical(vec);

    // Вывод количество символов
    std::cout << vec.size() << "\n";
    writer.WriteNumber(vec.size(), 9);

    // Вывод этих символов (их нужно выводить как они в ASCII формате записаны)
    std::map<size_t, size_t> sizes;
    for (auto& [len, symbol] : vec) {
        writer.Write(symbol);
        ++sizes[len];
    }

    // Вывод для каждой длины количество символов с такой длиной
    for (size_t i = 1; i <= sizes.rbegin()->first; ++i) {
        writer.WriteNumber(sizes[i], 9);
    }

    // Вывод имени файла в каноническом формате
    for (auto& element : elements) {
        writer.Write(canonical_codes[element]);
    }

    // Вывод служебного символа, что имя файла кончилось в каноническом формате
    writer.Write(canonical_codes[FILENAME_END]);

    // Вывод содержимого из файла тоже в каноническом формате
    while (true) {
        std::vector<bool> symbol = reader.Read(8);
        symbol.insert(symbol.begin(), 0);
        if (reader.IsEnd()) {
            break;
        }
        writer.Write(canonical_codes[symbol]);
    }

    // Вывод служебного символа тоже в каноническом формате
    if (end_archive) {
        writer.Write(canonical_codes[ARCHIVE_END]);
    } else {
        writer.Write(canonical_codes[ONE_MORE_FILE]);
    }

    delete huffman.release();
}
