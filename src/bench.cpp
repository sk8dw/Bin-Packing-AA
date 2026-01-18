#include <iostream>
#include "bin_packing.h"
#include "benchmark/benchmark.h"

using namespace std;

static void BM_test(benchmark::State& state) {
    int x = 0;
    for (auto _ : state) {
        cout<<1<<endl;
        int result = 1;
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_test);

BENCHMARK_MAIN();