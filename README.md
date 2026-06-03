# Лабораторная работа №3 — Range Query Data Structures

Реализация структур данных для задач RSQ (Range Sum Query) и RMQ (Range Minimum Query) с подсчётом ключевых операций и замером реального времени.

## Сборка

```bash
cmake -S . -B build
cmake --build build
```

Запуск бенчмарка:
```bash
./build/lab3
```

Запуск тестов:
```bash
./build/lab3_tests
```

Или через CTest:
```bash
cd build && ctest
```

## Структуры данных

### 1D RSQ — Prefix Sum (`PrefixSum1D`)

Префиксная сумма. `prefix[i] = a[0] + ... + a[i-1]`, запрос `[l, r] = prefix[r+1] - prefix[l]`.

| n | build ops | query ops |
|---|---|---|
| 100 | 100 | 1.00 |
| 1 000 | 1 000 | 1.00 |
| 100 000 | 100 000 | 1.00 |

**Build: O(n) &nbsp;·&nbsp; Query: O(1)**

---

### 2D RSQ — Prefix Sum (`PrefixSum2D`)

Двумерная префиксная сумма. Запрос прямоугольника через формулу включений-исключений.

| n×n | build ops | query ops |
|---|---|---|
| 10×10 | 100 | 3.00 |
| 100×100 | 10 000 | 3.00 |
| 1000×1000 | 1 000 000 | 3.00 |

**Build: O(n²) &nbsp;·&nbsp; Query: O(1)**

---

### 1D RMQ — All Segments (`RMQNaive`)

Предпросчёт минимума для всех пар `(l, r)`. `table[l][r] = min(table[l][r-1], a[r])`.

| n | build ops | query ops |
|---|---|---|
| 100 | 5 050 | 1.00 |
| 1 000 | 500 500 | 1.00 |
| 5 000 | 12 502 500 | 1.00 |

**Build: O(n²) &nbsp;·&nbsp; Query: O(1)**

---

### RSQ/RMQ — Sqrt Decomposition (`SqrtDecomposition<T, Op>`)

Шаблонный класс. Операция передаётся лямбдой, тип — через шаблонный параметр. Массив делится на блоки размером `√n`. Запрос: крайние неполные блоки обходятся поэлементно, средние — по блокам.

```cpp
auto sumOp = [](int a, int b) { return a + b; };
SqrtDecomposition<int, decltype(sumOp)> sq(sumOp, 0);
sq.build(arr);

auto minOp = [](int a, int b) { return std::min(a, b); };
SqrtDecomposition<int, decltype(minOp)> sq(minOp, INT_MAX);
```

| n | build ops | avg query ops |
|---|---|---|
| 100 | 100 | 13.2 |
| 1 000 | 1 000 | 41.0 |
| 10 000 | 10 000 | 132.7 |
| 100 000 | 100 000 | 415.8 |

√100 000 ≈ 316 — среднее выше из-за случайно длинных запросов.

**Build: O(n) &nbsp;·&nbsp; Query: O(√n) &nbsp;·&nbsp; Update: O(√n)**

---

### RSQ/RMQ — Segment Tree (`SegmentTree<T, Op>`)

Шаблонное дерево отрезков с поддержкой точечного обновления. Операция передаётся лямбдой.

| n | build ops | avg query ops | avg update ops |
|---|---|---|---|
| 100 | 199 | 14.4 | 7.7 |
| 1 000 | 1 999 | 24.1 | 11.0 |
| 10 000 | 19 999 | 34.2 | 14.3 |
| 100 000 | 199 999 | 44.2 | 17.7 |

log₂(100 000) ≈ 17 — значения совпадают.

**Build: O(n) &nbsp;·&nbsp; Query: O(log n) &nbsp;·&nbsp; Update: O(log n)**

---

### RSQ — Fenwick Tree (`FenwickTree`)

Дерево Фенвика (BIT). Каждый узел хранит сумму диапазона, определяемого младшим битом индекса. Поддерживает точечное обновление.

| n | build ops | avg query ops | avg update ops |
|---|---|---|---|
| 100 | 376 | 6.3 | 3.7 |
| 1 000 | 5 060 | 9.9 | 5.2 |
| 10 000 | 71 728 | 12.9 | 7.2 |
| 100 000 | 878 000 | 16.3 | 8.8 |

**Build: O(n log n) &nbsp;·&nbsp; Query: O(log n) &nbsp;·&nbsp; Update: O(log n)**

---

### RMQ — Sparse Table (`SparseTable`)

`table[k][i]` = минимум на `[i, i + 2ᵏ - 1]`. Запрос двумя перекрывающимися степенями двойки — всегда ровно одна операция сравнения.

| n | build ops | query ops |
|---|---|---|
| 100 | 580 | 1.00 |
| 1 000 | 8 987 | 1.00 |
| 100 000 | 1 568 946 | 1.00 |

**Build: O(n log n) &nbsp;·&nbsp; Query: O(1)**

---

### RMQ — Hybrid (`HybridRMQ`)

Комбинация корневой декомпозиции и разреженной таблицы. Массив делится на блоки размером `B = ⌊log₂(n) / 2⌋`. Внутри каждого блока строится разреженная таблица, поверх блочных минимумов — ещё одна. Это даёт O(n) построение при сохранении O(1) запроса.

Запрос `[l, r]`:
1. Левый неполный блок — внутренняя sparse table, O(1)
2. Средние полные блоки — внешняя sparse table, O(1)
3. Правый неполный блок — внутренняя sparse table, O(1)

| n | build ops | avg query ops |
|---|---|---|
| 100 | 347 | 2.88 |
| 1 000 | 4 003 | 2.97 |
| 10 000 | 41 300 | 3.00 |
| 100 000 | 433 631 | 3.00 |

Против SparseTable при n=100 000: build 433 631 vs 1 568 946 операций — выигрыш в 3.6×.

**Build: O(n) &nbsp;·&nbsp; Query: O(1)**

---

## Сравнение по реальному времени (скомпилировано с `-O3`)

| n | SparseTable build (ms) | HybridRMQ build (ms) | SparseTable query (ms) | HybridRMQ query (ms) |
|---|---|---|---|---|
| 1 000 | 0.013 | 0.032 | 1.732 | 2.469 |
| 10 000 | 0.058 | 0.196 | 1.742 | 2.591 |
| 100 000 | 2.694 | 3.616 | 1.986 | 6.642 |
| 500 000 | 23.435 | 18.574 | 3.353 | 14.447 |
| 1 000 000 | 50.293 | 41.406 | 5.023 | 17.614 |

На малых n SparseTable строится быстрее из-за меньшей константы. На n ≥ 500 000 HybridRMQ начинает выигрывать по построению, что подтверждает теоретическое O(n) vs O(n log n).

## Итоговая таблица сложностей

| Структура | Build | Query | Update |
|---|---|---|---|
| PrefixSum1D | O(n) | O(1) | — |
| PrefixSum2D | O(n²) | O(1) | — |
| RMQNaive | O(n²) | O(1) | — |
| SqrtDecomposition | O(n) | O(√n) | O(√n) |
| SegmentTree | O(n) | O(log n) | O(log n) |
| FenwickTree | O(n log n) | O(log n) | O(log n) |
| SparseTable | O(n log n) | O(1) | — |
| HybridRMQ | O(n) | O(1) | — |

## Тесты

23 теста, 7009 assertions — Catch2.

```
All tests passed (7009 assertions in 23 test cases)
```
