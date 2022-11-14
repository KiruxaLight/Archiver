#include "decompressor.h"
#include "archiver.h"
#include "duty.h"
#include "reading.h"
#include "writing.h"
#include "canonical.h"

#include <unordered_map>
#include <vector>

void Decompressor::Decompress(const std::string& name) {
    std::ifstream in(name, std::ios::binary);
    Reader reader(in);
    bool archive_end = false;
    while (true) {
        if (archive_end) {
            break;
        }
        std::vector<bool> symbol_count_str = reader.Read(9);
        size_t symbol_count = ToDec(symbol_count_str);
        std::vector<std::vector<bool>> symbols;
        for (size_t i = 0; i < symbol_count; ++i) {
            std::vector<bool> cur_symbol = reader.Read(9);
            symbols.push_back(cur_symbol);
        }
        std::vector<std::pair<size_t, std::vector<bool>>> vec;

        // Проходимся по списку длин и вот мы нашли ненулевое количество, тогда засунем в vec все слова с такой длиной
        // (пара <длина, символ>)
        for (size_t index = 0, len = 0, current = 0; index < symbol_count; ++index, --current) {
            while (current == 0) {
                std::vector<bool> cur = reader.Read(9);
                current = ToDec(cur);
                ++len;
            }
            vec.push_back({len, symbols[index]});
        }
        Canonical canonical;
        std::unordered_map<std::vector<bool>, std::vector<bool>> canonical_codes = canonical.ToCanonical(vec);

        // По каноническому коду получить символ, это будет нужно когда будем считывать название файла и содержимое
        std::unordered_map<std::vector<bool>, std::vector<bool>> table;
        for (const auto& [symbol, code] : canonical_codes) {
            table[code] = symbol;
        }

        // А здесь уже расшифровываем содержимое файла
        std::vector<bool> current_symbol;  // Текущий символ, который накапливается. Накапливается канонический код
        std::string cur_file_name;
        while (true) {
            // Вот мы считываем слова, а если слово это FILENAME_END, то конец названия файла
            if (table.find(current_symbol) != table.end() && table[current_symbol] == FILENAME_END) {
                break;
            }
            // Считываем по биту в каноническом формате пока можем, если находим, то это наше какое-то слово
            while (table.find(current_symbol) == table.end()) {
                std::vector<bool> code = reader.Read(1);
                bool current_bit = code[0];
                current_symbol.push_back(current_bit);
            }
            // Просто к названию файла добавляем символ, который обработали
            if (table[current_symbol] != FILENAME_END) {
                cur_file_name += static_cast<char>(ToDec(table[current_symbol]));
                current_symbol = std::vector<bool>();
            }
        }

        // Теперь должны просто вывести в конечный файл всё
        std::ofstream out(cur_file_name, std::ios::binary);
        Writer writer(out);
        current_symbol = std::vector<bool>();
        while (true) {
            if (table.find(current_symbol) != table.end() &&
                (table[current_symbol] == ONE_MORE_FILE || table[current_symbol] == ARCHIVE_END)) {
                break;
            }
            // Аналогично предыдущей части
            while (table.find(current_symbol) == table.end()) {
                std::vector<bool> code = reader.Read(1);
                bool current_bit = code[0];
                current_symbol.push_back(current_bit);
            }
            // А тут самое главное -- выписываем содержимое зашифрованного файла в наш файл
            if (table[current_symbol] != ARCHIVE_END && table[current_symbol] != ONE_MORE_FILE) {
                std::vector<bool> symbol = table[current_symbol];
                symbol.erase(symbol.begin());
                writer.Write(symbol);
                current_symbol = std::vector<bool>();
            }
        }
        // Обязательно дописать то, что не дописалось
        writer.Clear();
        // Если конец архива, то всё, а если ещё один файл, то продолжаем аналогичные действия со следующим файлом
        if (table[current_symbol] == ARCHIVE_END) {
            archive_end = true;
        }
    }
}
