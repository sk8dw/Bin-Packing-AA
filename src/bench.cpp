#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
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
    char full_path[150] = {0};
    char num[5] = {0};
    int n,w;
    for(int i = 0; i < 25 ; i++) {
        Tests t; // Fresh object for every file
        t.total_weight = 0;
        
        // Build path: tests/folder/test + i + .txt
        sprintf(full_path, "%s%d.txt", path.c_str(), i);
        
        ifstream fin(full_path);
        if (!fin) continue; // Skip if file doesn't exist

        fin >> n >> t.cap;
        for(int j = 0; j < n; j++) {
            fin >> w;
            t.packs.push_back({w, j});
            t.total_weight += w;
        }

        cout << endl;
        strcpy(t.name,full_path+6);
        
        tests.push_back(t);
        fin.close();
    }
}

void Load_tests() {
    char path[50] = {0};
    strcpy(path,"tests\\near-half\\test");
    load_from_path(path);
    strcpy(path,"tests\\random\\test");
    load_from_path(path);
    strcpy(path,"tests\\two-cat\\test");
    load_from_path(path);
}




// --- The Universal Template Wrapper ---
template <typename Func>
void BM_PackingWrapper(benchmark::State& state, Func algo_call, int packet_count) {
    int last_bins = 0;
    for (auto _ : state) {
        auto result = algo_call();
        last_bins = result.size(); // Store it here
        benchmark::DoNotOptimize(result);
    }
    state.counters["Bins"] = last_bins;
    state.counters["Packets"] = packet_count;
}

int main(int argc, char** argv) {
    // 1. Setup Test Data (In real code, load your folders here)
    Load_tests();
    char buff[200];
    for (auto& test : tests) {
        int cap = test.cap;
        auto& w = test.packs;
        double total = test.total_weight;

        // --- HEURISTICS ---
        strcpy(buff,"FirstFit\\");
        strcat(buff,test.name);
        benchmark::RegisterBenchmark(buff, 
            BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                return first_fit(cap, w); 
            }, test.packs.size());
        strcpy(buff,"FirstFitDecreasing\\");
        strcat(buff,test.name);
        benchmark::RegisterBenchmark(buff, 
            BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                return first_fit_decreasing(cap, w);
            }, test.packs.size());
        strcpy(buff,"BestFit\\");
        strcat(buff,test.name);
        benchmark::RegisterBenchmark(buff, 
            BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                return best_fit(cap, w);
            }, test.packs.size());
        strcpy(buff,"BestFitDecreasing\\");
        strcat(buff,test.name);
        benchmark::RegisterBenchmark(buff, 
            BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                return best_fit_decreasing(cap, w);
            }, test.packs.size());

        // --- BACKTRACKING VARIATIONS ---

        if (w.size() <= 21) { // Safety guard for Backtracking
            
            // 1. Not Sorted (Original Order)
            strcpy(buff,"Backtracking_Unsorted_no_prune\\");
            strcat(buff,test.name);
            benchmark::RegisterBenchmark(std::string(buff), 
                BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                    return backtracking(cap,test.packs,no_prune,test.total_weight);
                }, test.packs.size());
            strcpy(buff,"Backtracking_Unsorted_better_prune\\");
            strcat(buff,test.name);
            benchmark::RegisterBenchmark(string(buff), 
                BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                    return backtracking(cap,test.packs,found_better_prune,test.total_weight);
                }, test.packs.size()); 
            strcpy(buff,"Backtracking_Unsorted_best_prune\\");
            strcat(buff,test.name);
            benchmark::RegisterBenchmark(string(buff), 
                BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                    return backtracking(cap,test.packs,fully_optimized_prune,test.total_weight);
                }, test.packs.size());
            // 2. Sorted Descending (Largest to Smallest - Usually Fastest)
            strcpy(buff,"Backtracking_Sorted_asc_no_prune\\");
            strcat(buff,test.name);
            benchmark::RegisterBenchmark(string(buff), 
                BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                    return backtracking_sort(cap,test.packs,no_prune,test.total_weight,compare_packs_incr);
                }, test.packs.size());
            strcpy(buff,"Backtracking_Sorted_asc_better_prune\\");
            strcat(buff,test.name);
            benchmark::RegisterBenchmark(string(buff), 
                BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                    return backtracking_sort(cap,test.packs,found_better_prune,test.total_weight,compare_packs_incr);
                }, test.packs.size());
            strcpy(buff,"Backtracking_Sorted_asc_best_prune\\");
            strcat(buff,test.name);
            benchmark::RegisterBenchmark(string(buff), 
                BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                    return backtracking_sort(cap,test.packs,fully_optimized_prune,test.total_weight,compare_packs_incr);
                }, test.packs.size());
            // 3. Sorted Ascending (Smallest to Largest - Usually Slowest)
            strcpy(buff,"Backtracking_Sorted_desc_no_prune\\");
            strcat(buff,test.name);
            benchmark::RegisterBenchmark(string(buff), 
                BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                    return backtracking_sort(cap,test.packs,no_prune,test.total_weight,compare_packs_decr);
                }, test.packs.size());
            strcpy(buff,"Backtracking_Sorted_desc_better_prune\\");
            strcat(buff,test.name);
            benchmark::RegisterBenchmark(string(buff), 
                BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                    return backtracking_sort(cap,test.packs,found_better_prune,test.total_weight,compare_packs_decr);
                }, test.packs.size());
            strcpy(buff,"Backtracking_Sorted_desc_best_prune\\");
            strcat(buff,test.name);
            benchmark::RegisterBenchmark(string(buff), 
                BM_PackingWrapper<std::function<vector<bin>()>>, [=]() {
                    return backtracking_sort(cap,test.packs,fully_optimized_prune,test.total_weight,compare_packs_decr);
                }, test.packs.size());
        }
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}