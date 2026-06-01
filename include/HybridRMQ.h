#pragma once
#include <vector>
#include <cstddef>
#include <algorithm>
#include <cmath>
#include <limits>

// RMQ via Hybrid: Sqrt Decomposition + Sparse Table per block
//
// Key idea:
//   - Split array into blocks of size B = ceil(log2(n) / 2)
//   - Build a Sparse Table over block minimums  -> O(n/B * log(n/B)) = O(n)
//   - Build a Sparse Table inside each block    -> O(B * log(B)) * (n/B) = O(n)
//   - Total build: O(n), Query: O(1)
//
// This beats plain Sparse Table's O(n log n) build while keeping O(1) query.

class HybridRMQ {
public:
    // Builds the structure. Returns number of key operations.
    size_t build(const std::vector<int>& arr);

    // Returns minimum on [l, r] (0-indexed). Writes iteration count to ops.
    int query(int l, int r, size_t& ops) const;

private:
    // Fast O(1) RMQ inside a single block using its sparse table
    int blockQuery(int block, int l, int r, size_t& ops) const;

    // floor(log2) lookup
    int log2floor(int x) const { return log2_[x]; }

    size_t n_ = 0;
    size_t B_ = 0;          // block size
    size_t numBlocks_ = 0;

    std::vector<int> data_;
    std::vector<int> log2_; // precomputed floor(log2(i))

    // Sparse table over block minimums
    // blockST_[k][b] = min over 2^k blocks starting at block b
    std::vector<std::vector<int>> blockST_;

    // Sparse table inside each block
    // inST_[b][k][i] = min over arr[b*B + i .. b*B + i + 2^k - 1]
    std::vector<std::vector<std::vector<int>>> inST_;
};
