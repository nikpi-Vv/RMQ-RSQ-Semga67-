#include "SparseTable.h"
#include <algorithm>
#include <cmath>

size_t SparseTable::build(const std::vector<int>& arr) {
    n_ = arr.size();
    size_t ops = 0;

    int maxLog = static_cast<int>(std::log2(n_)) + 1;

    // Precompute floor(log2) for each index
    log2_.resize(n_ + 1, 0);
    for (size_t i = 2; i <= n_; ++i)
        log2_[i] = log2_[i / 2] + 1;

    // table_[k][i] = min of arr[i .. i + 2^k - 1]
    table_.assign(maxLog, std::vector<int>(n_));

    // Base case: intervals of length 1
    for (size_t i = 0; i < n_; ++i) {
        table_[0][i] = arr[i];
        ++ops;
    }

    // Fill by doubling
    for (int k = 1; k < maxLog; ++k) {
        for (size_t i = 0; i + (1 << k) <= n_; ++i) {
            table_[k][i] = std::min(table_[k - 1][i],
                                    table_[k - 1][i + (1 << (k - 1))]);
            ++ops;
        }
    }
    return ops;
}

int SparseTable::query(int l, int r, size_t& ops) const {
    // Two overlapping intervals of length 2^k cover [l, r] exactly
    int k = log2_[r - l + 1];
    ops = 1; // one min comparison
    return std::min(table_[k][l],
                    table_[k][r - (1 << k) + 1]);
}
