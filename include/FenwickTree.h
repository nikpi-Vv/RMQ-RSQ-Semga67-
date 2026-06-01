#pragma once
#include <vector>
#include <cstddef>

// RSQ via Fenwick Tree (Binary Indexed Tree)
// Build: O(n log n), Query: O(log n), Update: O(log n)
class FenwickTree {
public:
    // Builds Fenwick tree from array. Returns number of key operations.
    size_t build(const std::vector<int>& arr);

    // Returns sum on [l, r] (0-indexed). Writes iteration count to ops.
    int query(int l, int r, size_t& ops) const;

    // Point update: add delta to position i. Returns number of key operations.
    size_t update(int i, int delta);

private:
    int prefixSum(int i, size_t& ops) const;

    std::vector<long long> tree_; // 1-indexed internally
    size_t n_ = 0;
};
