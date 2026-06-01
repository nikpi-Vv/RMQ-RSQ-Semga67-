#pragma once
#include <vector>
#include <cstddef>

// 2D Range Sum Query via 2D prefix sum
// Build: O(n*m), Query: O(1)
class PrefixSum2D {
public:
    // Builds 2D prefix sum table. Returns number of key operations.
    size_t build(const std::vector<std::vector<int>>& grid);

    // Returns sum in rectangle (r1,c1)-(r2,c2) inclusive (0-indexed).
    // Writes iteration count to ops.
    int query(int r1, int c1, int r2, int c2, size_t& ops) const;

private:
    std::vector<std::vector<long long>> prefix_; // prefix_[i][j] = sum of grid[0..i-1][0..j-1]
};
