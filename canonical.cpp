#include "canonical.h"

std::unordered_map<std::vector<bool>, std::vector<bool>> Canonical::ToCanonical(
    std::vector<std::pair<size_t, std::vector<bool>>>& vec) {
    std::vector<bool> current_code(vec[0].first);
    std::unordered_map<std::vector<bool>, std::vector<bool>> table;
    for (size_t i = 0; i < vec.size(); ++i) {
        table[vec[i].second] = current_code;
        if (i + 1 < vec.size()) {
            size_t j = 0;
            for (j = current_code.size(); j > 0 && current_code[j - 1]; --j) {
                current_code[j - 1] = false;
            }
            if (j > 0) {
                current_code[j - 1] = true;
            } else {
                exit(111);  // asdasd
            }
            for (j = 0; j < vec[i + 1].first - vec[i].first; ++j) {
                current_code.push_back(false);
            }
        }
    }
    return table;
}
