#pragma once
#include <vector>
#include <cstddef>
#include <functional>

// Segment Tree — supports any associative operation
// Build: O(n), Query: O(log n), Update: O(log n)
//
// Template parameters:
//   T   - element type
//   Op  - binary operation type
template<typename T, typename Op>
class SegmentTree {
public:
    SegmentTree(Op op, T identity)
        : op_(op), identity_(identity) {}

    // Builds segment tree. Returns number of key operations.
    size_t build(const std::vector<T>& arr) {
        n_ = arr.size();
        tree_.assign(4 * n_, identity_);
        size_t ops = 0;
        build_(arr, 1, 0, n_ - 1, ops);
        return ops;
    }

    // Returns op-result on [l, r] (0-indexed). Writes iteration count to ops.
    T query(int l, int r, size_t& ops) const {
        ops = 0;
        return query_(1, 0, n_ - 1, l, r, ops);
    }

    // Point update: set position pos to value. Returns number of key operations.
    size_t update(int pos, T value) {
        size_t ops = 0;
        update_(1, 0, n_ - 1, pos, value, ops);
        return ops;
    }

private:
    void build_(const std::vector<T>& arr, int node, int lo, int hi, size_t& ops) {
        if (lo == hi) {
            tree_[node] = arr[lo];
            ++ops;
            return;
        }
        int mid = (lo + hi) / 2;
        build_(arr, 2 * node,     lo,      mid, ops);
        build_(arr, 2 * node + 1, mid + 1, hi,  ops);
        tree_[node] = op_(tree_[2 * node], tree_[2 * node + 1]);
        ++ops;
    }

    T query_(int node, int lo, int hi, int l, int r, size_t& ops) const {
        if (r < lo || hi < l) return identity_;
        ++ops;
        if (l <= lo && hi <= r) return tree_[node];
        int mid = (lo + hi) / 2;
        T left  = query_(2 * node,     lo,      mid, l, r, ops);
        T right = query_(2 * node + 1, mid + 1, hi,  l, r, ops);
        return op_(left, right);
    }

    void update_(int node, int lo, int hi, int pos, T value, size_t& ops) {
        ++ops;
        if (lo == hi) {
            tree_[node] = value;
            return;
        }
        int mid = (lo + hi) / 2;
        if (pos <= mid) update_(2 * node,     lo,      mid, pos, value, ops);
        else            update_(2 * node + 1, mid + 1, hi,  pos, value, ops);
        tree_[node] = op_(tree_[2 * node], tree_[2 * node + 1]);
    }

    Op op_;
    T identity_;
    size_t n_ = 0;
    std::vector<T> tree_;
};
