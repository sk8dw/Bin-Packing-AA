#include "bin_packing.h"

struct SearchState {
    int item_idx;
    int bin_idx;
};
void undo_move(vector<bin>& bins, int bin_idx, int weight);
void backtrack_last_item(vector<bin>& bins, vector<SearchState>& stack, vector<packet> pack);



vector<bin> backtracking(int capacity, vector<packet> pack, bool (*prune_cond)(size_t, size_t, double, double), double total_weight) {
    vector<bin> current_bins;
    vector<bin> best_sol;
    current_bins.reserve(pack.size());
    size_t min_bins = pack.size() + 1;
    vector<SearchState> search_stack;
    search_stack.push_back({0, 0});

    while (!search_stack.empty()) {
        SearchState& curr = search_stack.back();
        if (prune_cond(current_bins.size(), min_bins, total_weight,(double) capacity)) {
            search_stack.pop_back();
            if (!search_stack.empty()) {
                backtrack_last_item(current_bins, search_stack, pack);
            }
            continue;
        }

        int weight = pack[curr.item_idx].weight;

        if (curr.bin_idx < (int)current_bins.size()) {
            if (current_bins[curr.bin_idx].space_left >= weight) {
                current_bins[curr.bin_idx].space_left -= weight;
                current_bins[curr.bin_idx].packets.push_back({weight,curr.item_idx});
                
                if (curr.item_idx == (int)pack.size() - 1) {
                    if(current_bins.size() <= min_bins) {
                        min_bins = (int) current_bins.size();
                        best_sol = current_bins;
                    }
                    undo_move(current_bins, curr.bin_idx, weight);
                    curr.bin_idx++; 
                } else {
                    search_stack.push_back({curr.item_idx + 1, 0});
                }
            } else {
                curr.bin_idx++;
            }
        } else if (curr.bin_idx == (int)current_bins.size()) {
            current_bins.push_back({capacity - weight, {{weight, curr.item_idx}}});
            
            if (curr.item_idx == (int)pack.size() - 1) {
                if(current_bins.size() <= min_bins) {
                    min_bins = (int) current_bins.size();
                    best_sol = current_bins;
                }
                current_bins.pop_back();
                curr.bin_idx++;
            } else {
                search_stack.push_back({curr.item_idx + 1, 0});
            }
        } else {
            search_stack.pop_back();
            if (!search_stack.empty()) {
                backtrack_last_item(current_bins, search_stack, pack);
            }
        }
    }
    return best_sol;
}

vector<vector<bin>> backtracking_all(int capacity, vector<packet> pack, bool (*prune_cond)(size_t, size_t, double, double), double total_weight) {
    vector<bin> current_bins;
    vector<vector<bin>> best_sols;
    current_bins.reserve(pack.size());
    size_t min_bins = pack.size() + 1;
    vector<SearchState> search_stack;
    search_stack.push_back({0, 0});



    while (!search_stack.empty()) {
        SearchState& curr = search_stack.back();
        if (prune_cond(current_bins.size(), min_bins, total_weight,(double) capacity)) {
            search_stack.pop_back();
            if (!search_stack.empty()) {
                backtrack_last_item(current_bins, search_stack, pack);
            }
            continue;
        }

        int weight = pack[curr.item_idx].weight;
        
        if (curr.bin_idx < (int)current_bins.size()) {
            if (current_bins[curr.bin_idx].space_left >= weight) {
                current_bins[curr.bin_idx].space_left -= weight;
                current_bins[curr.bin_idx].packets.push_back({weight,curr.item_idx});
                
                if (curr.item_idx == (int)pack.size() - 1) {
                    if(current_bins.size() < min_bins) {
                        min_bins = (int) current_bins.size();
                        best_sols.clear();
                        best_sols.push_back(current_bins);
                    } else if (current_bins.size() == min_bins) {
                        best_sols.push_back(current_bins);
                    }
                    undo_move(current_bins, curr.bin_idx, weight);
                    curr.bin_idx++; 
                } else {
                    search_stack.push_back({curr.item_idx + 1, 0});
                }
            } else {
                curr.bin_idx++;
            }
        } else if (curr.bin_idx == (int)current_bins.size()) {
            current_bins.push_back({capacity - weight, {{weight, curr.item_idx}}});
            
            if (curr.item_idx == (int)pack.size() - 1) {
                if(current_bins.size() < min_bins) {
                    min_bins = (int) current_bins.size();
                    best_sols.clear();
                    best_sols.push_back(current_bins);
                } else if (current_bins.size() == min_bins) {
                    best_sols.push_back(current_bins);
                }
                current_bins.pop_back();
                curr.bin_idx++;
            } else {
                search_stack.push_back({curr.item_idx + 1, 0});
            }
        } else {
            search_stack.pop_back();
            if (!search_stack.empty()) {
                backtrack_last_item(current_bins, search_stack, pack);
            }
        }
    }
    return best_sols;
}

vector<vector<bin>> backtracking_all_sort(int capacity, vector<packet> pack, bool (*prune_cond)(size_t, size_t, double, double), double total_weight, bool (*compare_packs)(packet A, packet B)) {
    sort(pack.begin(), pack.end(),compare_packs);
    return backtracking_all(capacity,pack,prune_cond,total_weight);
}

vector<bin> backtracking_sort(int capacity, vector<packet> pack, bool (*prune_cond)(size_t, size_t, double, double), double total_weight, bool (*compare_packs)(packet A, packet B)) {
    sort(pack.begin(), pack.end(),compare_packs);
    return backtracking(capacity,pack,prune_cond,total_weight);
}

void undo_move(vector<bin>& bins, int bin_idx, int weight) {
    bins[bin_idx].space_left += weight;
    bins[bin_idx].packets.pop_back();
    if(bins[bin_idx].packets.empty()) { ///sigur bin_idx este ultimul si avem un bin doar cu penultimul pachet, bin care poate fi doar ultimul bin
        bins.pop_back();
    }
}

void backtrack_last_item(vector<bin>& bins, vector<SearchState>& stack, vector<packet> pack) {
    SearchState& prev = stack.back();
    int weight = pack[prev.item_idx].weight;
    if (prev.bin_idx < (int)bins.size()) {
        undo_move(bins, prev.bin_idx, weight);
    } else {
        bins.pop_back();
    }
    prev.bin_idx++;
}

bool no_prune(size_t nr_of_bins, size_t min_bins, double total_weight, double cap){
    return false;
}

bool found_better_prune_all(size_t nr_of_bins, size_t min_bins, double total_weight, double cap) {
    return nr_of_bins > min_bins;
}

bool fully_optimized_prune_all(size_t nr_of_bins, size_t min_bins, double total_weight, double cap) {
    return nr_of_bins > min_bins || nr_of_bins >= total_weight / cap * 2 + 1;
}

bool found_better_prune(size_t nr_of_bins, size_t min_bins, double total_weight, double cap) {
    return nr_of_bins >= min_bins;
}

bool fully_optimized_prune(size_t nr_of_bins, size_t min_bins, double total_weight, double cap) {
    return nr_of_bins >= min_bins || nr_of_bins >= total_weight / cap * 2 + 1;
}