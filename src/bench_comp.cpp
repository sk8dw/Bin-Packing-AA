#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include "bin_packing.h"
#include "benchmark/benchmark.h"

using namespace std;

struct Tests {
    char name[100];
    int cap;
    vector<packet> packs;
    double total_weight;
};

vector<Tests> tests;

void load_from_path(const string& path) {
    char full_path[150];
    int n, w_val;
    
    for(int i = 0; i < 25; i++) {
        Tests t; // Fresh object for every file
        t.total_weight = 0;
        
        // Build path: tests/folder/test + i + .txt
        sprintf(full_path, "%s%d.txt", path.c_str(), i);
        
        ifstream fin(full_path);
        if (!fin) continue; // Skip if file doesn't exist

        fin >> n >> t.cap;
        for(int j = 0; j < n; j++) {
            fin >> w_val;
            t.packs.push_back({w_val, j});
            t.total_weight += w_val;
        }

        // Clean up the name for the benchmark label (removes "tests/")
        string clean_name = full_path;
        size_t last_slash = clean_name.find_last_of("\\/");
        if (last_slash != string::npos) {
            strcpy(t.name, clean_name.substr(last_slash + 1).c_str());
        } else {
            strcpy(t.name, full_path);
        }
        
        tests.push_back(t);
        fin.close();
    }
}

void Load_tests() {
    tests.clear();
    load_from_path("tests/near-half/test");
    load_from_path("tests/random/test");
    load_from_path("tests/two-cat/test");
}

// --- The Optimized Universal Template Wrapper ---
template <typename Func>
void BM_PackingWrapper(benchmark::State& state, Func algo_call) {
    int last_bin_count = 0;
    for (auto _ : state) {
        auto result = algo_call();
        last_bin_count = (int)result.size();
        benchmark::DoNotOptimize(result);
    }
    // Set counter without re-running the algorithm
    state.counters["Bins"] = last_bin_count;
}

int main(int argc, char** argv) {
    Load_tests();

    for (auto& test : tests) {
        // Capture specific test data locally for the lambdas
        // Note: Capturing by value [=] ensures data safety
        
        // --- HEURISTICS ---
        benchmark::RegisterBenchmark((string("FF/") + test.name).c_str(), 
            BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                return first_fit(test.cap, test.packs); 
            });

        benchmark::RegisterBenchmark((string("FFD/") + test.name).c_str(), 
            BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                return first_fit_decreasing(test.cap, test.packs);
            });

        benchmark::RegisterBenchmark((string("BFD/") + test.name).c_str(), 
            BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                return best_fit_decreasing(test.cap, test.packs);
            });

        // --- BACKTRACKING VARIATIONS ---
        if (test.packs.size() <= 20) {
            // Pruning Variation: Best
            benchmark::RegisterBenchmark((string("BT_BestPrune/") + test.name).c_str(), 
                BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                    return backtracking(test.cap, test.packs, fully_optimized_prune, test.total_weight);
                });

            // Sorting Variation: Descending (Fastest)
            benchmark::RegisterBenchmark((string("BT_SortDesc/") + test.name).c_str(), 
                BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                    return backtracking_sort(test.cap, test.packs, fully_optimized_prune, test.total_weight, compare_packs_decr);
                });

            // Sorting Variation: Ascending (Slowest)
            benchmark::RegisterBenchmark((string("BT_SortAsc/") + test.name).c_str(), 
                BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                    return backtracking_sort(test.cap, test.packs, fully_optimized_prune, test.total_weight, compare_packs_incr);
                });
        }
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}