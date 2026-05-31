#include "PrefixSum2D.h"

size_t PrefixSum2D::build(const std::vector<std::vector<int>>& grid) {
    size_t ops = 0;
    int n = static_cast<int>(grid.size());
    int m = n > 0 ? static_cast<int>(grid[0].size()) : 0;

    prefix_.assign(n + 1, std::vector<long long>(m + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            prefix_[i][j] = grid[i - 1][j - 1]
                           + prefix_[i - 1][j]
                           + prefix_[i][j - 1]
                           - prefix_[i - 1][j - 1];
            ++ops; // one inclusion-exclusion per cell
        }
    }
    return ops;
}

int PrefixSum2D::query(int r1, int c1, int r2, int c2, size_t& ops) const {
    // inclusion-exclusion formula: 3 additions/subtractions
    ops = 3;
    long long result = prefix_[r2 + 1][c2 + 1]
                     - prefix_[r1][c2 + 1]
                     - prefix_[r2 + 1][c1]
                     + prefix_[r1][c1];
    return static_cast<int>(result);
}
