"""
Генерирует графики по данным бенчмарка.
Запуск: python3 docs/plot.py
Графики сохраняются в docs/img/
"""

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import os

OUT = os.path.join(os.path.dirname(__file__), "img")
os.makedirs(OUT, exist_ok=True)

STYLE = {
    "figure.facecolor": "white",
    "axes.facecolor": "#f8f9fa",
    "axes.grid": True,
    "grid.color": "white",
    "grid.linewidth": 1.2,
    "axes.spines.top": False,
    "axes.spines.right": False,
    "font.family": "DejaVu Sans",
    "font.size": 11,
}
plt.rcParams.update(STYLE)

COLORS = ["#4C72B0", "#DD8452", "#55A868", "#C44E52", "#8172B2", "#937860", "#DA8BC3", "#8C8C8C"]

# ── данные ──────────────────────────────────────────────────────────────────

n_common   = [100, 500, 1_000, 5_000, 10_000, 50_000, 100_000]
n_2d       = [10, 50, 100, 200, 500, 1_000]
n_naive    = [50, 100, 300, 500, 1_000, 2_000, 5_000]
n_timing   = [1_000, 10_000, 100_000, 500_000, 1_000_000]

build_prefix1d  = [100, 500, 1_000, 5_000, 10_000, 50_000, 100_000]
build_prefix2d  = [100, 2_500, 10_000, 40_000, 250_000, 1_000_000]
build_naive     = [1_275, 5_050, 45_150, 125_250, 500_500, 2_001_000, 12_502_500]
build_sqrt      = [100, 500, 1_000, 5_000, 10_000, 50_000, 100_000]
build_segtree   = [199, 999, 1_999, 9_999, 19_999, 99_999, 199_999]
build_fenwick   = [376, 2_280, 5_060, 33_364, 71_728, 414_000, 878_000]
build_sparse    = [580, 3_998, 8_987, 56_822, 123_631, 734_481, 1_568_946]
build_hybrid    = [347, 1_880, 4_003, 19_826, 41_300, 213_252, 433_631]

query_prefix1d  = [1.00] * 7
query_naive     = [1.00] * 7
query_sqrt      = [13.15, 28.77, 40.97, 94.41, 132.67, 297.30, 416.24]
query_segtree   = [14.41, 20.93, 24.07, 31.26, 34.22, 41.09, 44.15]
query_fenwick   = [6.33, 8.93, 9.92, 11.82, 12.94, 15.15, 16.28]
query_sparse    = [1.00] * 7
query_hybrid    = [2.88, 2.96, 2.97, 3.00, 3.00, 3.00, 3.00]

update_segtree  = [7.72, 9.98, 10.98, 13.38, 14.34, 16.69, 17.69]
update_fenwick  = [3.70, 4.50, 5.15, 6.71, 7.16, 8.33, 8.84]

st_build_ms  = [0.013, 0.058, 2.694, 23.435, 50.293]
hr_build_ms  = [0.032, 0.196, 3.616, 18.574, 41.406]
st_query_ms  = [1.732, 1.742, 1.986,  3.353,  5.023]
hr_query_ms  = [2.469, 2.591, 6.642, 14.447, 17.614]

# ── helpers ──────────────────────────────────────────────────────────────────

def save(name):
    path = os.path.join(OUT, name)
    plt.tight_layout()
    plt.savefig(path, dpi=150, bbox_inches="tight")
    plt.close()
    print(f"  saved {path}")


def fmt_n(x, _):
    if x >= 1_000_000: return f"{x/1_000_000:.0f}M"
    if x >= 1_000:     return f"{x/1_000:.0f}k"
    return str(int(x))

# ── Plot 1: Build ops — RSQ structures ───────────────────────────────────────

fig, ax = plt.subplots(figsize=(8, 5))
ax.plot(n_common, build_prefix1d, "o-", color=COLORS[0], label="PrefixSum1D  O(n)")
ax.plot(n_common, build_segtree,  "s-", color=COLORS[1], label="SegmentTree  O(n)")
ax.plot(n_common, build_sqrt,     "^-", color=COLORS[2], label="SqrtDecomp   O(n)")
ax.plot(n_common, build_fenwick,  "D-", color=COLORS[3], label="FenwickTree  O(n log n)")
ax.set_title("Построение RSQ-структур — количество операций")
ax.set_xlabel("n")
ax.set_ylabel("операций")
ax.xaxis.set_major_formatter(ticker.FuncFormatter(fmt_n))
ax.yaxis.set_major_formatter(ticker.FuncFormatter(fmt_n))
ax.legend()
save("build_rsq.png")

# ── Plot 2: Build ops — RMQ structures ───────────────────────────────────────

fig, ax = plt.subplots(figsize=(8, 5))
ax.plot(n_common, build_segtree, "s-", color=COLORS[1], label="SegmentTree  O(n)")
ax.plot(n_common, build_hybrid,  "P-", color=COLORS[4], label="HybridRMQ   O(n)")
ax.plot(n_common, build_sparse,  "v-", color=COLORS[2], label="SparseTable  O(n log n)")
ax.plot(n_naive,  build_naive,   "x--",color=COLORS[3], label="RMQNaive     O(n²)")
ax.set_title("Построение RMQ-структур — количество операций")
ax.set_xlabel("n")
ax.set_ylabel("операций")
ax.xaxis.set_major_formatter(ticker.FuncFormatter(fmt_n))
ax.yaxis.set_major_formatter(ticker.FuncFormatter(fmt_n))
ax.legend()
save("build_rmq.png")

# ── Plot 3: Query ops — все структуры ────────────────────────────────────────

fig, ax = plt.subplots(figsize=(8, 5))
ax.plot(n_common, query_sqrt,    "^-", color=COLORS[0], label="SqrtDecomp   O(√n)")
ax.plot(n_common, query_segtree, "s-", color=COLORS[1], label="SegmentTree  O(log n)")
ax.plot(n_common, query_fenwick, "D-", color=COLORS[3], label="FenwickTree  O(log n)")
ax.plot(n_common, query_hybrid,  "P-", color=COLORS[4], label="HybridRMQ   O(1)")
ax.plot(n_common, query_sparse,  "v-", color=COLORS[2], label="SparseTable  O(1)")
ax.plot(n_common, query_prefix1d,"o-", color=COLORS[5], label="PrefixSum1D  O(1)")
ax.set_title("Запрос — среднее количество операций")
ax.set_xlabel("n")
ax.set_ylabel("операций")
ax.xaxis.set_major_formatter(ticker.FuncFormatter(fmt_n))
ax.legend()
save("query_ops.png")

# ── Plot 4: Update ops ────────────────────────────────────────────────────────

fig, ax = plt.subplots(figsize=(8, 5))
ax.plot(n_common, update_segtree, "s-", color=COLORS[1], label="SegmentTree  O(log n)")
ax.plot(n_common, update_fenwick, "D-", color=COLORS[3], label="FenwickTree  O(log n)")
ax.set_title("Обновление точки — среднее количество операций")
ax.set_xlabel("n")
ax.set_ylabel("операций")
ax.xaxis.set_major_formatter(ticker.FuncFormatter(fmt_n))
ax.legend()
save("update_ops.png")

# ── Plot 5: Wall-clock build time ─────────────────────────────────────────────

fig, ax = plt.subplots(figsize=(8, 5))
ax.plot(n_timing, st_build_ms, "v-", color=COLORS[2], label="SparseTable  O(n log n)")
ax.plot(n_timing, hr_build_ms, "P-", color=COLORS[4], label="HybridRMQ   O(n)")
ax.set_title("Время построения (мс) при -O3")
ax.set_xlabel("n")
ax.set_ylabel("мс")
ax.xaxis.set_major_formatter(ticker.FuncFormatter(fmt_n))
ax.legend()
save("timing_build.png")

# ── Plot 6: Wall-clock query time ─────────────────────────────────────────────

fig, ax = plt.subplots(figsize=(8, 5))
ax.plot(n_timing, st_query_ms, "v-", color=COLORS[2], label="SparseTable")
ax.plot(n_timing, hr_query_ms, "P-", color=COLORS[4], label="HybridRMQ")
ax.set_title("Время 100 000 запросов (мс) при -O3")
ax.set_xlabel("n")
ax.set_ylabel("мс")
ax.xaxis.set_major_formatter(ticker.FuncFormatter(fmt_n))
ax.legend()
save("timing_query.png")

print("Все графики сохранены в docs/img/")
