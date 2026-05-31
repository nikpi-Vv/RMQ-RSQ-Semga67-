#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include <limits>

#include "SqrtDecomposition.h"
#include "SegmentTree.h"

std::vector<int> randomVector(int n) {
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(-1000, 1000);
    std::vector<int> v(n);
    for (auto& x : v) x = dist(rng);
    return v;
}

// ---- Sqrt Decomposition: RSQ ----
void benchmark_SqrtRSQ() {
    std::cout << "\n=== RSQ via Sqrt Decomposition ===\n";
    std::cout << std::setw(8)  << "n"
              << std::setw(14) << "build_ops"
              << std::setw(18) << "avg_query_ops" << "\n";

    auto sumOp = [](int a, int b) { return a + b; };
    std::vector<int> sizes = {100, 500, 1000, 5000, 10000, 50000, 100000};
    std::mt19937 rng(7);

    for (int n : sizes) {
        auto arr = randomVector(n);
        SqrtDecomposition<int, decltype(sumOp)> sq(sumOp, 0);
        size_t buildOps = sq.build(arr);

        std::uniform_int_distribution<int> dist(0, n - 1);
        double totalOps = 0;
        const int Q = 1000;
        for (int q = 0; q < Q; ++q) {
            int l = dist(rng), r = dist(rng);
            if (l > r) std::swap(l, r);
            size_t ops = 0;
            sq.query(l, r, ops);
            totalOps += ops;
        }

        std::cout << std::setw(8)  << n
                  << std::setw(14) << buildOps
                  << std::setw(18) << std::fixed << std::setprecision(2)
                  << totalOps / Q << "\n";
    }
}

// ---- Sqrt Decomposition: RMQ ----
void benchmark_SqrtRMQ() {
    std::cout << "\n=== RMQ via Sqrt Decomposition ===\n";
    std::cout << std::setw(8)  << "n"
              << std::setw(14) << "build_ops"
              << std::setw(18) << "avg_query_ops" << "\n";

    auto minOp = [](int a, int b) { return std::min(a, b); };
    std::vector<int> sizes = {100, 500, 1000, 5000, 10000, 50000, 100000};
    std::mt19937 rng(7);

    for (int n : sizes) {
        auto arr = randomVector(n);
        SqrtDecomposition<int, decltype(minOp)> sq(minOp, std::numeric_limits<int>::max());
        size_t buildOps = sq.build(arr);

        std::uniform_int_distribution<int> dist(0, n - 1);
        double totalOps = 0;
        const int Q = 1000;
        for (int q = 0; q < Q; ++q) {
            int l = dist(rng), r = dist(rng);
            if (l > r) std::swap(l, r);
            size_t ops = 0;
            sq.query(l, r, ops);
            totalOps += ops;
        }

        std::cout << std::setw(8)  << n
                  << std::setw(14) << buildOps
                  << std::setw(18) << std::fixed << std::setprecision(2)
                  << totalOps / Q << "\n";
    }
}

// ---- Segment Tree: RSQ ----
void benchmark_SegTreeRSQ() {
    std::cout << "\n=== RSQ via Segment Tree ===\n";
    std::cout << std::setw(8)  << "n"
              << std::setw(14) << "build_ops"
              << std::setw(18) << "avg_query_ops"
              << std::setw(18) << "avg_update_ops" << "\n";

    auto sumOp = [](int a, int b) { return a + b; };
    std::vector<int> sizes = {100, 500, 1000, 5000, 10000, 50000, 100000};
    std::mt19937 rng(7);

    for (int n : sizes) {
        auto arr = randomVector(n);
        SegmentTree<int, decltype(sumOp)> st(sumOp, 0);
        size_t buildOps = st.build(arr);

        std::uniform_int_distribution<int> distIdx(0, n - 1);
        std::uniform_int_distribution<int> distVal(-1000, 1000);
        double totalQueryOps = 0, totalUpdateOps = 0;
        const int Q = 1000;
        for (int q = 0; q < Q; ++q) {
            int l = distIdx(rng), r = distIdx(rng);
            if (l > r) std::swap(l, r);
            size_t ops = 0;
            st.query(l, r, ops);
            totalQueryOps += ops;

            size_t uops = st.update(distIdx(rng), distVal(rng));
            totalUpdateOps += uops;
        }

        std::cout << std::setw(8)  << n
                  << std::setw(14) << buildOps
                  << std::setw(18) << std::fixed << std::setprecision(2)
                  << totalQueryOps / Q
                  << std::setw(18) << totalUpdateOps / Q << "\n";
    }
}

// ---- Segment Tree: RMQ ----
void benchmark_SegTreeRMQ() {
    std::cout << "\n=== RMQ via Segment Tree ===\n";
    std::cout << std::setw(8)  << "n"
              << std::setw(14) << "build_ops"
              << std::setw(18) << "avg_query_ops"
              << std::setw(18) << "avg_update_ops" << "\n";

    auto minOp = [](int a, int b) { return std::min(a, b); };
    std::vector<int> sizes = {100, 500, 1000, 5000, 10000, 50000, 100000};
    std::mt19937 rng(7);

    for (int n : sizes) {
        auto arr = randomVector(n);
        SegmentTree<int, decltype(minOp)> st(minOp, std::numeric_limits<int>::max());
        size_t buildOps = st.build(arr);

        std::uniform_int_distribution<int> distIdx(0, n - 1);
        std::uniform_int_distribution<int> distVal(-1000, 1000);
        double totalQueryOps = 0, totalUpdateOps = 0;
        const int Q = 1000;
        for (int q = 0; q < Q; ++q) {
            int l = distIdx(rng), r = distIdx(rng);
            if (l > r) std::swap(l, r);
            size_t ops = 0;
            st.query(l, r, ops);
            totalQueryOps += ops;

            size_t uops = st.update(distIdx(rng), distVal(rng));
            totalUpdateOps += uops;
        }

        std::cout << std::setw(8)  << n
                  << std::setw(14) << buildOps
                  << std::setw(18) << std::fixed << std::setprecision(2)
                  << totalQueryOps / Q
                  << std::setw(18) << totalUpdateOps / Q << "\n";
    }
}

int main() {
    benchmark_SqrtRSQ();
    benchmark_SqrtRMQ();
    benchmark_SegTreeRSQ();
    benchmark_SegTreeRMQ();
    return 0;
}
