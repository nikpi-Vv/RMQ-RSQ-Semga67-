#pragma once
#include <vector>
#include <cstddef>

// 1D Range Sum Query via prefix sum
// Build: O(n), Query: O(1)
class PrefixSum1D {
public:
    // Builds prefix sum array. Returns number of key operations.
    size_t build(const std::vector<int>& arr);

    // Returns sum on [l, r] (0-indexed). Writes iteration count to ops.
    int query(int l, int r, size_t& ops) const;

private:
    std::vector<long long> prefix_; // prefix_[i] = a[0] + ... + a[i-1]
};
