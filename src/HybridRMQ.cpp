#include "HybridRMQ.h"

size_t HybridRMQ::build(const std::vector<int>& arr) {
    size_t ops = 0;
    n_ = arr.size();
    data_ = arr;

    // Block size: ceil(log2(n) / 2), minimum 1
    B_ = std::max(size_t(1), static_cast<size_t>(std::log2(n_) / 2));
    numBlocks_ = (n_ + B_ - 1) / B_;

    // Precompute floor(log2)
    log2_.resize(n_ + 1, 0);
    for (size_t i = 2; i <= n_; ++i)
        log2_[i] = log2_[i / 2] + 1;

    // --- Step 1: sparse table inside each block ---
    inST_.resize(numBlocks_);
    for (size_t b = 0; b < numBlocks_; ++b) {
        size_t start = b * B_;
        size_t end   = std::min(start + B_, n_);
        size_t bLen  = end - start;

        int maxLog = log2_[bLen] + 1;
        inST_[b].assign(maxLog, std::vector<int>(bLen, std::numeric_limits<int>::max()));

        // Base
        for (size_t i = 0; i < bLen; ++i) {
            inST_[b][0][i] = data_[start + i];
            ++ops;
        }
        // Fill by doubling
        for (int k = 1; k < maxLog; ++k) {
            for (size_t i = 0; i + (1 << k) <= bLen; ++i) {
                inST_[b][k][i] = std::min(inST_[b][k-1][i],
                                           inST_[b][k-1][i + (1 << (k-1))]);
                ++ops;
            }
        }
    }

    // --- Step 2: compute block minimums ---
    std::vector<int> blockMin(numBlocks_);
    for (size_t b = 0; b < numBlocks_; ++b) {
        blockMin[b] = inST_[b][0][0]; // min of first element (base); real min at level 0 span
        // actually take the true block min: level log of full block
        size_t start = b * B_;
        size_t end   = std::min(start + B_, n_);
        size_t bLen  = end - start;
        int k = log2_[bLen];
        blockMin[b] = std::min(inST_[b][k][0],
                               inST_[b][k][bLen - (1 << k)]);
        ++ops;
    }

    // --- Step 3: sparse table over block minimums ---
    int maxLogB = log2_[numBlocks_] + 1;
    blockST_.assign(maxLogB, std::vector<int>(numBlocks_, std::numeric_limits<int>::max()));

    for (size_t b = 0; b < numBlocks_; ++b) {
        blockST_[0][b] = blockMin[b];
        ++ops;
    }
    for (int k = 1; k < maxLogB; ++k) {
        for (size_t b = 0; b + (1 << k) <= numBlocks_; ++b) {
            blockST_[k][b] = std::min(blockST_[k-1][b],
                                       blockST_[k-1][b + (1 << (k-1))]);
            ++ops;
        }
    }

    return ops;
}

int HybridRMQ::blockQuery(int block, int l, int r, size_t& ops) const {
    // l, r are local indices within the block
    int len = r - l + 1;
    int k = log2_[len];
    ++ops;
    return std::min(inST_[block][k][l],
                    inST_[block][k][r - (1 << k) + 1]);
}

int HybridRMQ::query(int l, int r, size_t& ops) const {
    ops = 0;
    int bL = l / B_;
    int bR = r / B_;

    if (bL == bR) {
        // Same block
        return blockQuery(bL, l - bL * B_, r - bL * B_, ops);
    }

    // Left partial block
    int result = blockQuery(bL, l - bL * B_, B_ - 1, ops);

    // Full blocks in the middle via block sparse table
    if (bL + 1 <= bR - 1) {
        int lo = bL + 1, hi = bR - 1;
        int k = log2_[hi - lo + 1];
        result = std::min(result,
                          std::min(blockST_[k][lo],
                                   blockST_[k][hi - (1 << k) + 1]));
        ++ops;
    }

    // Right partial block
    size_t bRstart = bR * B_;
    result = std::min(result, blockQuery(bR, 0, r - bRstart, ops));

    return result;
}
