#ifndef PACK
#define PACK
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
using namespace std;
struct packet {
    int weight ;
    int index;
};
struct bin {
    int space_left;
    vector<packet> packets;
};
vector<vector<bin>> backtracking_all(int capacity, vector<packet> pack, bool (*prune_cond)(size_t, size_t, double, double), double total_weight);
vector<bin> backtracking(int capacity, vector<packet> pack, bool (*prune_cond)(size_t, size_t, double, double), double total_weight);
vector<vector<bin>> backtracking_all_sort(int capacity, vector<packet> pack, bool (*prune_cond)(size_t, size_t, double, double), double total_weight, bool (*compare_packs)(packet A, packet B));
vector<bin> backtracking_sort(int capacity, vector<packet> pack, bool (*prune_cond)(size_t, size_t, double, double), double total_weight, bool (*compare_packs)(packet A, packet B));
vector<bin> first_fit(int cap, vector<packet> pack);
vector<bin> first_fit_decreasing(int cap, vector<packet> pack);
vector<bin> best_fit(int cap, vector<packet> pack);
vector<bin> best_fit_decreasing(int cap, vector<packet> pack);
bool no_prune(size_t nr_of_bins, size_t min_bins, double total_weight, double cap);
bool found_better_prune(size_t nr_of_bins, size_t min_bins, double total_weight, double cap);
bool fully_optimized_prune(size_t nr_of_bins, size_t min_bins, double total_weight, double cap);
bool found_better_prune_all(size_t nr_of_bins, size_t min_bins, double total_weight, double cap);
bool fully_optimized_prune_all(size_t nr_of_bins, size_t min_bins, double total_weight, double cap);
bool compare_packs_decr(packet A, packet B);
bool compare_packs_incr(packet A, packet B);
#endif