#include <benchmark/benchmark.h>

#include <random>

#include "rdc/rdc.h"

void BM_rdc(benchmark::State& state) {
  constexpr size_t size = 1'000;

  std::mt19937 gen(42);
  std::uniform_real_distribution<> dist(std::numeric_limits<double>().min(),
                                        std::numeric_limits<double>().max());

  std::vector<double> x_values, y_values;
  x_values.reserve(size);
  y_values.reserve(size);

  for (size_t i = 0; i < size; i++) {
    x_values.emplace_back(dist(gen));
    y_values.emplace_back(dist(gen));
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(rdc::rdc(x_values, y_values));
  }
}

BENCHMARK(BM_rdc);

BENCHMARK_MAIN();