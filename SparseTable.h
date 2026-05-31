#pragma once
#include <vector>
#include <cstddef>

// RMQ via Sparse Table
// Build: O(n log n), Query: O(1)
class SparseTable {
public:
    // Builds sparse table. Returns number of key operations.
    size_t build(const std::vector<int>& arr);

    // Returns minimum on [l, r] (0-indexed). Writes iteration count to ops.
    int query(int l, int r, size_t& ops) const;

private:
    // table_[k][i] = min of arr[i .. i + 2^k - 1]
    std::vector<std::vector<int>> table_;
    std::vector<int> log2_;   // precomputed floor(log2(i))
    size_t n_ = 0;
};
