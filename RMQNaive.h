#pragma once
#include <vector>
#include <cstddef>

// 1D Range Minimum Query via precomputed all segments
// Build: O(n^2), Query: O(1)
class RMQNaive {
public:
    // Precomputes minimum for every (l, r) pair. Returns number of key operations.
    size_t build(const std::vector<int>& arr);

    // Returns minimum on [l, r] (0-indexed). Writes iteration count to ops.
    int query(int l, int r, size_t& ops) const;

private:
    // table_[l][r] = min of arr[l..r]
    std::vector<std::vector<int>> table_;
};
