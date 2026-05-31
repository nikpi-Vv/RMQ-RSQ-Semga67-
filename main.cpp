#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include <limits>

#include "PrefixSum1D.h"
#include "PrefixSum2D.h"
#include "RMQNaive.h"
#include "SqrtDecomposition.h"
#include "SegmentTree.h"
#include "FenwickTree.h"
#include "SparseTable.h"

// ---- helpers ----

std::vector<int> randomVector(int n) {
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(-1000, 1000);
    std::vector<int> v(n);
    for (auto& x : v) x = dist(rng);
    return v;
}

std::vector<std::vector<int>> randomGrid(int n) {
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(-1000, 1000);
    std::vector<std::vector<int>> g(n, std::vector<int>(n));
    for (auto& row : g)
        for (auto& x : row) x = dist(rng);
    return g;
}

void printHeader(bool hasUpdate = false) {
    std::cout << std::setw(8)  << "n"
              << std::setw(16) << "build_ops"
              << std::setw(18) << "avg_query_ops";
    if (hasUpdate)
        std::cout << std::setw(19) << "avg_update_ops";
    std::cout << "\n";
}

// ---- benchmarks ----

void bench_PrefixSum1D() {
    std::cout << "\n=== 1D RSQ (Prefix Sum) | Build: O(n) | Query: O(1) ===\n";
    printHeader();

    std::vector<int> sizes = {100, 500, 1000, 5000, 10000, 50000, 100000};
    std::mt19937 rng(7);

    for (int n : sizes) {
        auto arr = randomVector(n);
        PrefixSum1D ps;
        size_t buildOps = ps.build(arr);

        std::uniform_int_distribution<int> dist(0, n - 1);
        double totalOps = 0;
        const int Q = 1000;
        for (int q = 0; q < Q; ++q) {
            int l = dist(rng), r = dist(rng);
            if (l > r) std::swap(l, r);
            size_t ops = 0;
            ps.query(l, r, ops);
            totalOps += ops;
        }
        std::cout << std::setw(8)  << n
                  << std::setw(16) << buildOps
                  << std::setw(18) << std::fixed << std::setprecision(2)
                  << totalOps / Q << "\n";
    }
}

void bench_PrefixSum2D() {
    std::cout << "\n=== 2D RSQ (Prefix Sum) | Build: O(n^2) | Query: O(1) ===\n";
    printHeader();

    std::vector<int> sizes = {10, 50, 100, 200, 500, 1000};
    std::mt19937 rng(7);

    for (int n : sizes) {
        auto grid = randomGrid(n);
        PrefixSum2D ps;
        size_t buildOps = ps.build(grid);

        std::uniform_int_distribution<int> dist(0, n - 1);
        double totalOps = 0;
        const int Q = 1000;
        for (int q = 0; q < Q; ++q) {
            int r1 = dist(rng), c1 = dist(rng);
            int r2 = dist(rng), c2 = dist(rng);
            if (r1 > r2) std::swap(r1, r2);
            if (c1 > c2) std::swap(c1, c2);
            size_t ops = 0;
            ps.query(r1, c1, r2, c2, ops);
            totalOps += ops;
        }
        std::cout << std::setw(8)  << n
                  << std::setw(16) << buildOps
                  << std::setw(18) << std::fixed << std::setprecision(2)
                  << totalOps / Q << "\n";
    }
}

void bench_RMQNaive() {
    std::cout << "\n=== 1D RMQ (All Segments) | Build: O(n^2) | Query: O(1) ===\n";
    printHeader();

    std::vector<int> sizes = {50, 100, 300, 500, 1000, 2000, 5000};
    std::mt19937 rng(7);

    for (int n : sizes) {
        auto arr = randomVector(n);
        RMQNaive rmq;
        size_t buildOps = rmq.build(arr);

        std::uniform_int_distribution<int> dist(0, n - 1);
        double totalOps = 0;
        const int Q = 1000;
        for (int q = 0; q < Q; ++q) {
            int l = dist(rng), r = dist(rng);
            if (l > r) std::swap(l, r);
            size_t ops = 0;
            rmq.query(l, r, ops);
            totalOps += ops;
        }
        std::cout << std::setw(8)  << n
                  << std::setw(16) << buildOps
                  << std::setw(18) << std::fixed << std::setprecision(2)
                  << totalOps / Q << "\n";
    }
}

void bench_SqrtDecomposition() {
    auto sumOp = [](int a, int b) { return a + b; };
    auto minOp = [](int a, int b) { return std::min(a, b); };

    std::vector<int> sizes = {100, 500, 1000, 5000, 10000, 50000, 100000};
    std::mt19937 rng(7);

    for (int pass = 0; pass < 2; ++pass) {
        if (pass == 0)
            std::cout << "\n=== RSQ via Sqrt Decomposition | Build: O(n) | Query: O(sqrt(n)) ===\n";
        else
            std::cout << "\n=== RMQ via Sqrt Decomposition | Build: O(n) | Query: O(sqrt(n)) ===\n";
        printHeader();

        for (int n : sizes) {
            auto arr = randomVector(n);
            size_t buildOps;
            double totalOps = 0;
            const int Q = 1000;
            std::uniform_int_distribution<int> dist(0, n - 1);

            if (pass == 0) {
                SqrtDecomposition<int, decltype(sumOp)> sq(sumOp, 0);
                buildOps = sq.build(arr);
                for (int q = 0; q < Q; ++q) {
                    int l = dist(rng), r = dist(rng);
                    if (l > r) std::swap(l, r);
                    size_t ops = 0;
                    sq.query(l, r, ops);
                    totalOps += ops;
                }
            } else {
                SqrtDecomposition<int, decltype(minOp)> sq(minOp, std::numeric_limits<int>::max());
                buildOps = sq.build(arr);
                for (int q = 0; q < Q; ++q) {
                    int l = dist(rng), r = dist(rng);
                    if (l > r) std::swap(l, r);
                    size_t ops = 0;
                    sq.query(l, r, ops);
                    totalOps += ops;
                }
            }
            std::cout << std::setw(8)  << n
                      << std::setw(16) << buildOps
                      << std::setw(18) << std::fixed << std::setprecision(2)
                      << totalOps / Q << "\n";
        }
    }
}

void bench_SegmentTree() {
    auto sumOp = [](int a, int b) { return a + b; };
    auto minOp = [](int a, int b) { return std::min(a, b); };

    std::vector<int> sizes = {100, 500, 1000, 5000, 10000, 50000, 100000};
    std::mt19937 rng(7);

    for (int pass = 0; pass < 2; ++pass) {
        if (pass == 0)
            std::cout << "\n=== RSQ via Segment Tree | Build: O(n) | Query: O(log n) | Update: O(log n) ===\n";
        else
            std::cout << "\n=== RMQ via Segment Tree | Build: O(n) | Query: O(log n) | Update: O(log n) ===\n";
        printHeader(true);

        for (int n : sizes) {
            auto arr = randomVector(n);
            size_t buildOps;
            double totalQueryOps = 0, totalUpdateOps = 0;
            const int Q = 1000;
            std::uniform_int_distribution<int> distIdx(0, n - 1);
            std::uniform_int_distribution<int> distVal(-1000, 1000);

            if (pass == 0) {
                SegmentTree<int, decltype(sumOp)> st(sumOp, 0);
                buildOps = st.build(arr);
                for (int q = 0; q < Q; ++q) {
                    int l = distIdx(rng), r = distIdx(rng);
                    if (l > r) std::swap(l, r);
                    size_t ops = 0;
                    st.query(l, r, ops);
                    totalQueryOps += ops;
                    totalUpdateOps += st.update(distIdx(rng), distVal(rng));
                }
            } else {
                SegmentTree<int, decltype(minOp)> st(minOp, std::numeric_limits<int>::max());
                buildOps = st.build(arr);
                for (int q = 0; q < Q; ++q) {
                    int l = distIdx(rng), r = distIdx(rng);
                    if (l > r) std::swap(l, r);
                    size_t ops = 0;
                    st.query(l, r, ops);
                    totalQueryOps += ops;
                    totalUpdateOps += st.update(distIdx(rng), distVal(rng));
                }
            }
            std::cout << std::setw(8)  << n
                      << std::setw(16) << buildOps
                      << std::setw(18) << std::fixed << std::setprecision(2)
                      << totalQueryOps / Q
                      << std::setw(19) << totalUpdateOps / Q << "\n";
        }
    }
}

void bench_FenwickTree() {
    std::cout << "\n=== RSQ via Fenwick Tree | Build: O(n log n) | Query: O(log n) | Update: O(log n) ===\n";
    printHeader(true);

    std::vector<int> sizes = {100, 500, 1000, 5000, 10000, 50000, 100000};
    std::mt19937 rng(7);

    for (int n : sizes) {
        auto arr = randomVector(n);
        FenwickTree ft;
        size_t buildOps = ft.build(arr);

        std::uniform_int_distribution<int> distIdx(0, n - 1);
        std::uniform_int_distribution<int> distVal(-100, 100);
        double totalQueryOps = 0, totalUpdateOps = 0;
        const int Q = 1000;
        for (int q = 0; q < Q; ++q) {
            int l = distIdx(rng), r = distIdx(rng);
            if (l > r) std::swap(l, r);
            size_t ops = 0;
            ft.query(l, r, ops);
            totalQueryOps += ops;
            totalUpdateOps += ft.update(distIdx(rng), distVal(rng));
        }
        std::cout << std::setw(8)  << n
                  << std::setw(16) << buildOps
                  << std::setw(18) << std::fixed << std::setprecision(2)
                  << totalQueryOps / Q
                  << std::setw(19) << totalUpdateOps / Q << "\n";
    }
}

void bench_SparseTable() {
    std::cout << "\n=== RMQ via Sparse Table | Build: O(n log n) | Query: O(1) ===\n";
    printHeader();

    std::vector<int> sizes = {100, 500, 1000, 5000, 10000, 50000, 100000};
    std::mt19937 rng(7);

    for (int n : sizes) {
        auto arr = randomVector(n);
        SparseTable st;
        size_t buildOps = st.build(arr);

        std::uniform_int_distribution<int> dist(0, n - 1);
        double totalOps = 0;
        const int Q = 1000;
        for (int q = 0; q < Q; ++q) {
            int l = dist(rng), r = dist(rng);
            if (l > r) std::swap(l, r);
            size_t ops = 0;
            st.query(l, r, ops);
            totalOps += ops;
        }
        std::cout << std::setw(8)  << n
                  << std::setw(16) << buildOps
                  << std::setw(18) << std::fixed << std::setprecision(2)
                  << totalOps / Q << "\n";
    }
}

int main() {
    bench_PrefixSum1D();
    bench_PrefixSum2D();
    bench_RMQNaive();
    bench_SqrtDecomposition();
    bench_SegmentTree();
    bench_FenwickTree();
    bench_SparseTable();
    return 0;
}
