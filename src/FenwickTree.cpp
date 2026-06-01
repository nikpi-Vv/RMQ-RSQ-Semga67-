#include "FenwickTree.h"

size_t FenwickTree::build(const std::vector<int>& arr) {
    n_ = arr.size();
    tree_.assign(n_ + 1, 0);
    size_t ops = 0;
    for (size_t i = 0; i < n_; ++i) {
        // update position i with arr[i]
        int pos = static_cast<int>(i) + 1;
        int val = arr[i];
        while (pos <= static_cast<int>(n_)) {
            tree_[pos] += val;
            pos += pos & (-pos); // move to next responsible node
            ++ops;
        }
    }
    return ops;
}

int FenwickTree::prefixSum(int i, size_t& ops) const {
    long long sum = 0;
    int pos = i + 1; // convert to 1-indexed
    while (pos > 0) {
        sum += tree_[pos];
        pos -= pos & (-pos); // move to parent
        ++ops;
    }
    return static_cast<int>(sum);
}

int FenwickTree::query(int l, int r, size_t& ops) const {
    ops = 0;
    int right = prefixSum(r, ops);
    int left  = (l > 0) ? prefixSum(l - 1, ops) : 0;
    return right - left;
}

size_t FenwickTree::update(int i, int delta) {
    size_t ops = 0;
    int pos = i + 1;
    while (pos <= static_cast<int>(n_)) {
        tree_[pos] += delta;
        pos += pos & (-pos);
        ++ops;
    }
    return ops;
}
