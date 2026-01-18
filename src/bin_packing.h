#ifndef PACK
#define PACK
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
using namespace std;
struct packet {
    int weight;
    int index;
};
struct bin {
    int space_left;
    vector<packet> packets;
};
vector<vector<bin>> backtracking_all(int n, int capacity, int weights[], bool (*prune_cond)(int, int, double, double), double total_weight);
vector<bin> backtracking(int n, int capacity, int weights[], bool (*prune_cond)(int, int, double, double), double total_weight);
vector<bin> first_fit(int n, int cap, vector<packet> pack);
vector<bin> first_fit_decreasing(int n, int cap, vector<packet> pack);
vector<bin> best_fit(int n, int cap, vector<packet> pack);
vector<bin> best_fit_decreasing(int n, int cap, vector<packet> pack);
bool no_prune(int nr_of_bins, int min_bins, double total_weight, double cap);
bool found_better_prune(int nr_of_bins, int min_bins, double total_weight, double cap);
bool fully_optimized_prune(int nr_of_bins, int min_bins, double total_weight, double cap);
bool compare_packs_decr(packet A, packet B);
#endif