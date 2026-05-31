#include "PrefixSum1D.h"

size_t PrefixSum1D::build(const std::vector<int>& arr) {
    size_t ops = 0;
    prefix_.resize(arr.size() + 1, 0);
    for (size_t i = 0; i < arr.size(); ++i) {
        prefix_[i + 1] = prefix_[i] + arr[i];
        ++ops; // one addition per element
    }
    return ops;
}

int PrefixSum1D::query(int l, int r, size_t& ops) const {
    ops = 1; // one subtraction: prefix[r+1] - prefix[l]
    return static_cast<int>(prefix_[r + 1] - prefix_[l]);
}
