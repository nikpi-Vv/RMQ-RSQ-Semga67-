#include <catch2/catch_all.hpp>
#include <vector>
#include <algorithm>
#include <limits>
#include <numeric>
#include <random>

#include "PrefixSum1D.h"
#include "PrefixSum2D.h"
#include "RMQNaive.h"
#include "SqrtDecomposition.h"
#include "SegmentTree.h"
#include "FenwickTree.h"
#include "SparseTable.h"
#include "HybridRMQ.h"

// ---- helpers ----

static int bruteSum(const std::vector<int>& a, int l, int r) {
    int s = 0;
    for (int i = l; i <= r; ++i) s += a[i];
    return s;
}

static int bruteMin(const std::vector<int>& a, int l, int r) {
    int m = std::numeric_limits<int>::max();
    for (int i = l; i <= r; ++i) m = std::min(m, a[i]);
    return m;
}

static int bruteSum2D(const std::vector<std::vector<int>>& g, int r1, int c1, int r2, int c2) {
    int s = 0;
    for (int i = r1; i <= r2; ++i)
        for (int j = c1; j <= c2; ++j)
            s += g[i][j];
    return s;
}

// Random queries helper
struct RNG {
    std::mt19937 rng{123};
    std::pair<int,int> range(int n) {
        std::uniform_int_distribution<int> d(0, n - 1);
        int a = d(rng), b = d(rng);
        if (a > b) std::swap(a, b);
        return {a, b};
    }
    int val(int lo, int hi) {
        return std::uniform_int_distribution<int>(lo, hi)(rng);
    }
};

// ===================== PrefixSum1D =====================

TEST_CASE("PrefixSum1D: single element") {
    PrefixSum1D ps;
    ps.build({42});
    size_t ops;
    REQUIRE(ps.query(0, 0, ops) == 42);
}

TEST_CASE("PrefixSum1D: full range sum") {
    std::vector<int> a = {1, 2, 3, 4, 5};
    PrefixSum1D ps;
    ps.build(a);
    size_t ops;
    REQUIRE(ps.query(0, 4, ops) == 15);
}

TEST_CASE("PrefixSum1D: random queries match brute force") {
    RNG rng;
    std::vector<int> a(200);
    for (auto& x : a) x = rng.val(-500, 500);
    PrefixSum1D ps;
    ps.build(a);
    for (int q = 0; q < 500; ++q) {
        auto [l, r] = rng.range(200);
        size_t ops;
        REQUIRE(ps.query(l, r, ops) == bruteSum(a, l, r));
    }
}

TEST_CASE("PrefixSum1D: build ops count equals n") {
    std::vector<int> a(100, 1);
    PrefixSum1D ps;
    REQUIRE(ps.build(a) == 100);
}

// ===================== PrefixSum2D =====================

TEST_CASE("PrefixSum2D: single cell") {
    PrefixSum2D ps;
    ps.build({{7}});
    size_t ops;
    REQUIRE(ps.query(0, 0, 0, 0, ops) == 7);
}

TEST_CASE("PrefixSum2D: full grid sum") {
    std::vector<std::vector<int>> g = {{1,2},{3,4}};
    PrefixSum2D ps;
    ps.build(g);
    size_t ops;
    REQUIRE(ps.query(0, 0, 1, 1, ops) == 10);
}

TEST_CASE("PrefixSum2D: random queries match brute force") {
    RNG rng;
    int n = 20;
    std::vector<std::vector<int>> g(n, std::vector<int>(n));
    for (auto& row : g) for (auto& x : row) x = rng.val(-100, 100);
    PrefixSum2D ps;
    ps.build(g);
    for (int q = 0; q < 300; ++q) {
        auto [r1, r2] = rng.range(n);
        auto [c1, c2] = rng.range(n);
        size_t ops;
        REQUIRE(ps.query(r1, c1, r2, c2, ops) == bruteSum2D(g, r1, c1, r2, c2));
    }
}

// ===================== RMQNaive =====================

TEST_CASE("RMQNaive: single element") {
    RMQNaive rmq;
    rmq.build({-5});
    size_t ops;
    REQUIRE(rmq.query(0, 0, ops) == -5);
}

TEST_CASE("RMQNaive: random queries match brute force") {
    RNG rng;
    std::vector<int> a(100);
    for (auto& x : a) x = rng.val(-1000, 1000);
    RMQNaive rmq;
    rmq.build(a);
    for (int q = 0; q < 500; ++q) {
        auto [l, r] = rng.range(100);
        size_t ops;
        REQUIRE(rmq.query(l, r, ops) == bruteMin(a, l, r));
    }
}

// ===================== SqrtDecomposition =====================

TEST_CASE("SqrtDecomposition RSQ: random queries match brute force") {
    auto sumOp = [](int a, int b){ return a + b; };
    RNG rng;
    std::vector<int> a(300);
    for (auto& x : a) x = rng.val(-500, 500);

    SqrtDecomposition<int, decltype(sumOp)> sq(sumOp, 0);
    sq.build(a);

    for (int q = 0; q < 500; ++q) {
        auto [l, r] = rng.range(300);
        size_t ops;
        REQUIRE(sq.query(l, r, ops) == bruteSum(a, l, r));
    }
}

TEST_CASE("SqrtDecomposition RMQ: random queries match brute force") {
    auto minOp = [](int a, int b){ return std::min(a, b); };
    RNG rng;
    std::vector<int> a(300);
    for (auto& x : a) x = rng.val(-500, 500);

    SqrtDecomposition<int, decltype(minOp)> sq(minOp, std::numeric_limits<int>::max());
    sq.build(a);

    for (int q = 0; q < 500; ++q) {
        auto [l, r] = rng.range(300);
        size_t ops;
        REQUIRE(sq.query(l, r, ops) == bruteMin(a, l, r));
    }
}

TEST_CASE("SqrtDecomposition RSQ: point update") {
    auto sumOp = [](int a, int b){ return a + b; };
    std::vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    SqrtDecomposition<int, decltype(sumOp)> sq(sumOp, 0);
    sq.build(a);
    sq.update(4, 100); // было 5, стало 100
    a[4] = 100;
    RNG rng;
    for (int q = 0; q < 200; ++q) {
        auto [l, r] = rng.range(10);
        size_t ops;
        REQUIRE(sq.query(l, r, ops) == bruteSum(a, l, r));
    }
}

// ===================== SegmentTree =====================

TEST_CASE("SegmentTree RSQ: random queries and updates") {
    auto sumOp = [](int a, int b){ return a + b; };
    RNG rng;
    std::vector<int> a(200);
    for (auto& x : a) x = rng.val(-500, 500);

    SegmentTree<int, decltype(sumOp)> st(sumOp, 0);
    st.build(a);

    for (int q = 0; q < 400; ++q) {
        auto [l, r] = rng.range(200);
        size_t ops;
        REQUIRE(st.query(l, r, ops) == bruteSum(a, l, r));
        // random point update
        int idx = rng.val(0, 199);
        int val = rng.val(-500, 500);
        st.update(idx, val);
        a[idx] = val;
    }
}

TEST_CASE("SegmentTree RMQ: random queries and updates") {
    auto minOp = [](int a, int b){ return std::min(a, b); };
    RNG rng;
    std::vector<int> a(200);
    for (auto& x : a) x = rng.val(-500, 500);

    SegmentTree<int, decltype(minOp)> st(minOp, std::numeric_limits<int>::max());
    st.build(a);

    for (int q = 0; q < 400; ++q) {
        auto [l, r] = rng.range(200);
        size_t ops;
        REQUIRE(st.query(l, r, ops) == bruteMin(a, l, r));
        int idx = rng.val(0, 199);
        int val = rng.val(-500, 500);
        st.update(idx, val);
        a[idx] = val;
    }
}

// ===================== FenwickTree =====================

TEST_CASE("FenwickTree: single element") {
    FenwickTree ft;
    ft.build({99});
    size_t ops;
    REQUIRE(ft.query(0, 0, ops) == 99);
}

TEST_CASE("FenwickTree: random queries match brute force") {
    RNG rng;
    std::vector<int> a(300);
    for (auto& x : a) x = rng.val(-500, 500);
    FenwickTree ft;
    ft.build(a);
    for (int q = 0; q < 500; ++q) {
        auto [l, r] = rng.range(300);
        size_t ops;
        REQUIRE(ft.query(l, r, ops) == bruteSum(a, l, r));
    }
}

TEST_CASE("FenwickTree: point update") {
    std::vector<int> a = {1, 2, 3, 4, 5};
    FenwickTree ft;
    ft.build(a);
    ft.update(2, 10); // a[2] += 10 → было 3, стало 13
    a[2] += 10;
    RNG rng;
    for (int q = 0; q < 100; ++q) {
        auto [l, r] = rng.range(5);
        size_t ops;
        REQUIRE(ft.query(l, r, ops) == bruteSum(a, l, r));
    }
}

// ===================== SparseTable =====================

TEST_CASE("SparseTable: single element") {
    SparseTable st;
    st.build({-7});
    size_t ops;
    REQUIRE(st.query(0, 0, ops) == -7);
}

TEST_CASE("SparseTable: random queries match brute force") {
    RNG rng;
    std::vector<int> a(500);
    for (auto& x : a) x = rng.val(-1000, 1000);
    SparseTable st;
    st.build(a);
    for (int q = 0; q < 1000; ++q) {
        auto [l, r] = rng.range(500);
        size_t ops;
        REQUIRE(st.query(l, r, ops) == bruteMin(a, l, r));
    }
}

TEST_CASE("SparseTable: query ops always 1") {
    std::vector<int> a(1000, 0);
    SparseTable st;
    st.build(a);
    RNG rng;
    for (int q = 0; q < 100; ++q) {
        auto [l, r] = rng.range(1000);
        size_t ops;
        st.query(l, r, ops);
        REQUIRE(ops == 1);
    }
}

// ===================== HybridRMQ =====================

TEST_CASE("HybridRMQ: single element") {
    HybridRMQ rmq;
    rmq.build({3});
    size_t ops;
    REQUIRE(rmq.query(0, 0, ops) == 3);
}

TEST_CASE("HybridRMQ: random queries match brute force") {
    RNG rng;
    std::vector<int> a(500);
    for (auto& x : a) x = rng.val(-1000, 1000);
    HybridRMQ rmq;
    rmq.build(a);
    for (int q = 0; q < 1000; ++q) {
        auto [l, r] = rng.range(500);
        size_t ops;
        REQUIRE(rmq.query(l, r, ops) == bruteMin(a, l, r));
    }
}

TEST_CASE("HybridRMQ: matches SparseTable on large input") {
    RNG rng;
    std::vector<int> a(10000);
    for (auto& x : a) x = rng.val(-1000, 1000);
    HybridRMQ hybrid;
    hybrid.build(a);
    SparseTable sparse;
    sparse.build(a);
    for (int q = 0; q < 1000; ++q) {
        auto [l, r] = rng.range(10000);
        size_t ops1, ops2;
        REQUIRE(hybrid.query(l, r, ops1) == sparse.query(l, r, ops2));
    }
}
