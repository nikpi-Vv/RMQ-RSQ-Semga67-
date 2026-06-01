#pragma once
#include <vector>
#include <cstddef>
#include <functional>
#include <cmath>

// Range Query via Square Root Decomposition
// Supports any associative operation passed as lambda
// Build: O(n), Query: O(sqrt(n)), Update: O(1)
//
// Template parameters:
//   T   - element type
//   Op  - binary operation type (e.g. std::function<T(T,T)> or lambda)
template<typename T, typename Op>
class SqrtDecomposition {
public:
    // op     - associative operation: (T, T) -> T
    // identity - neutral element for op (e.g. 0 for sum, INT_MAX for min)
    SqrtDecomposition(Op op, T identity)
        : op_(op), identity_(identity) {}

    // Builds block structure. Returns number of key operations.
    size_t build(const std::vector<T>& arr) {
        size_t ops = 0;
        n_ = arr.size();
        blockSize_ = static_cast<size_t>(std::sqrt(n_)) + 1;
        data_ = arr;

        size_t numBlocks = (n_ + blockSize_ - 1) / blockSize_;
        blocks_.assign(numBlocks, identity_);

        for (size_t i = 0; i < n_; ++i) {
            blocks_[i / blockSize_] = op_(blocks_[i / blockSize_], data_[i]);
            ++ops;
        }
        return ops;
    }

    // Returns op-result on [l, r] (0-indexed). Writes iteration count to ops.
    T query(int l, int r, size_t& ops) const {
        ops = 0;
        T result = identity_;
        int blockL = l / blockSize_;
        int blockR = r / blockSize_;

        if (blockL == blockR) {
            // Same block — scan elements directly
            for (int i = l; i <= r; ++i) {
                result = op_(result, data_[i]);
                ++ops;
            }
        } else {
            // Left partial block
            for (int i = l; i < static_cast<int>((blockL + 1) * blockSize_); ++i) {
                result = op_(result, data_[i]);
                ++ops;
            }
            // Full blocks in the middle
            for (int b = blockL + 1; b < blockR; ++b) {
                result = op_(result, blocks_[b]);
                ++ops;
            }
            // Right partial block
            for (int i = static_cast<int>(blockR * blockSize_); i <= r; ++i) {
                result = op_(result, data_[i]);
                ++ops;
            }
        }
        return result;
    }

    // Point update: set data_[i] = value. Returns number of key operations.
    size_t update(int i, T value) {
        data_[i] = value;
        // Recompute the block
        size_t b = i / blockSize_;
        size_t start = b * blockSize_;
        size_t end = std::min(start + blockSize_, n_);
        blocks_[b] = identity_;
        for (size_t j = start; j < end; ++j)
            blocks_[b] = op_(blocks_[b], data_[j]);
        return blockSize_; // O(sqrt(n)) ops
    }

private:
    Op op_;
    T identity_;
    size_t n_ = 0;
    size_t blockSize_ = 0;
    std::vector<T> data_;
    std::vector<T> blocks_;
};
