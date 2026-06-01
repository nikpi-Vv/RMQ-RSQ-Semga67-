#include "RMQNaive.h"
#include <algorithm>
#include <limits>

size_t RMQNaive::build(const std::vector<int>& arr) {
    size_t ops = 0;
    int n = static_cast<int>(arr.size());

    table_.assign(n, std::vector<int>(n, std::numeric_limits<int>::max()));

    // Fill diagonal (single elements)
    for (int i = 0; i < n; ++i) {
        table_[i][i] = arr[i];
        ++ops;
    }

    // Fill all segments of length >= 2
    for (int l = 0; l < n; ++l) {
        for (int r = l + 1; r < n; ++r) {
            table_[l][r] = std::min(table_[l][r - 1], arr[r]);
            ++ops; // one min comparison per cell
        }
    }
    return ops;
}

int RMQNaive::query(int l, int r, size_t& ops) const {
    ops = 1; // direct table lookup
    return table_[l][r];
}
