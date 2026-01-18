#include "bin_packing.h"

struct SearchState {
    int item_idx;
    int bin_idx;
};
void undo_move(vector<bin>& bins, int bin_idx, int weight);
void backtrack_last_item(vector<bin>& bins, vector<SearchState>& stack, int weights[]);
vector<bin> backtracking(int n, int capacity, int weights[], bool (*prune_cond)(int, int, double, double), double total_weight) {
    vector<bin> current_bins;
    vector<bin> best_sol;
    current_bins.reserve(n);
    int min_bins = n + 1;
    // We use a vector as a stack for the search states (faster than std::stack)
    vector<SearchState> search_stack;
    search_stack.push_back({0, 0});

    while (!search_stack.empty()) {
        SearchState& curr = search_stack.back();

        // PRUNING: If we already use more bins than our best solution, stop this branch
        if (prune_cond((int)current_bins.size(), min_bins, total_weight,(double) capacity)) {
            search_stack.pop_back();
            if (!search_stack.empty()) {
                backtrack_last_item(current_bins, search_stack, weights);
            }
            continue;
        }

        // Try placing the item
        int weight = weights[curr.item_idx];
        
        // bin_idx == current_bins.size() means "Try opening a new bin"
        if (curr.bin_idx < (int)current_bins.size()) {
            // Try putting in existing bin
            if (current_bins[curr.bin_idx].space_left >= weight) {
                // SUCCESS: Place and move to next item
                current_bins[curr.bin_idx].space_left -= weight;
                current_bins[curr.bin_idx].packets.push_back({weight,curr.item_idx});
                
                if (curr.item_idx == n - 1) {
                    //save_best_solution(current_bins, best_sol, min_bins);
                    if((int)current_bins.size() <= min_bins) {
                        min_bins = (int) current_bins.size();
                        best_sol = current_bins;
                    }
                    // Undo to keep searching for even better ones
                    undo_move(current_bins, curr.bin_idx, weight);
                    curr.bin_idx++; 
                } else {
                    search_stack.push_back({curr.item_idx + 1, 0});
                }
            } else {
                // Failed to fit, try next bin
                curr.bin_idx++;
            }
        } else if (curr.bin_idx == (int)current_bins.size()) {
            // SUCCESS: Open new bin
            current_bins.push_back({capacity - weight, {{weight, curr.item_idx}}});
            
            if (curr.item_idx == n - 1) {
                ///save_best_solution(current_bins, best_sol, min_bins);
                if((int)current_bins.size() <= min_bins) {
                    min_bins = (int) current_bins.size();
                    best_sol = current_bins;
                }
                current_bins.pop_back();
                curr.bin_idx++;
            } else {
                search_stack.push_back({curr.item_idx + 1, 0});
            }
        } else {
            // EXHAUSTED: All bins tried for this item, must backtrack
            search_stack.pop_back();
            if (!search_stack.empty()) {
                backtrack_last_item(current_bins, search_stack, weights);
            }
        }
    }
    return best_sol;
}

vector<vector<bin>> backtracking_all(int n, int capacity, int weights[], bool (*prune_cond)(int, int, double, double), double total_weight) {
    vector<bin> current_bins;
    vector<vector<bin>> best_sols;
    current_bins.reserve(n);
    int min_bins = n + 1;
    // We use a vector as a stack for the search states (faster than std::stack)
    vector<SearchState> search_stack;
    search_stack.push_back({0, 0});



    while (!search_stack.empty()) {
        SearchState& curr = search_stack.back();
        //std::cout<< curr.item_idx<< " " << curr.bin_idx<< " " << current_bins.size()<<endl;
        // PRUNING: If we already use more bins than our best solution, stop this branch
        if (prune_cond((int)current_bins.size(), min_bins, total_weight,(double) capacity)) {
            search_stack.pop_back();
            if (!search_stack.empty()) {
                backtrack_last_item(current_bins, search_stack, weights);
            }
            continue;
        }

        // Try placing the item
        int weight = weights[curr.item_idx];
        
        // bin_idx == current_bins.size() means "Try opening a new bin"
        if (curr.bin_idx < (int)current_bins.size()) {
            // Try putting in existing bin
            if (current_bins[curr.bin_idx].space_left >= weight) {
                // SUCCESS: Place and move to next item
                current_bins[curr.bin_idx].space_left -= weight;
                current_bins[curr.bin_idx].packets.push_back({weight,curr.item_idx});
                
                if (curr.item_idx == n - 1) {
                    //save_best_solution(current_bins, best_sol, min_bins);
                    if((int)current_bins.size() < min_bins) {
                        min_bins = (int) current_bins.size();
                        best_sols.clear();
                        best_sols.push_back(current_bins);
                    } else if ((int)current_bins.size() == min_bins) {
                        best_sols.push_back(current_bins);
                    }
                    // Undo to keep searching for even better ones
                    undo_move(current_bins, curr.bin_idx, weight);
                    curr.bin_idx++; 
                } else {
                    search_stack.push_back({curr.item_idx + 1, 0});
                }
            } else {
                // Failed to fit, try next bin
                curr.bin_idx++;
            }
        } else if (curr.bin_idx == (int)current_bins.size()) {
            // SUCCESS: Open new bin
            current_bins.push_back({capacity - weight, {{weight, curr.item_idx}}});
            
            if (curr.item_idx == n - 1) {
                ///save_best_solution(current_bins, best_sol, min_bins);
                if((int)current_bins.size() < min_bins) {
                    min_bins = (int) current_bins.size();
                    best_sols.clear();
                    best_sols.push_back(current_bins);
                } else if ((int)current_bins.size() == min_bins) {
                    best_sols.push_back(current_bins);
                }
                current_bins.pop_back();
                curr.bin_idx++;
            } else {
                search_stack.push_back({curr.item_idx + 1, 0});
            }
        } else {
            // EXHAUSTED: All bins tried for this item, must backtrack
            search_stack.pop_back();
            if (!search_stack.empty()) {
                backtrack_last_item(current_bins, search_stack, weights);
            }
        }
    }
    return best_sols;
}

void undo_move(vector<bin>& bins, int bin_idx, int weight) {
    bins[bin_idx].space_left += weight;
    bins[bin_idx].packets.pop_back();
    if(bins[bin_idx].packets.empty()) { ///sigur bin_idx este ultimul si avem un bin doar cu penultimul pachet, bin care poate fi doar ultimul bin
        bins.pop_back();
    }
}

void backtrack_last_item(vector<bin>& bins, vector<SearchState>& stack, int weights[]) {
    SearchState& prev = stack.back();
    int weight = weights[prev.item_idx];
    if (prev.bin_idx < (int)bins.size()) {
        undo_move(bins, prev.bin_idx, weight);
    } else {
        bins.pop_back(); // It was in its own new bin
    }
    prev.bin_idx++; // Try the next option for the item we just removed
}

bool no_prune(int nr_of_bins, int min_bins, double total_weight, double cap){
    return false;
}

bool found_better_prune(int nr_of_bins, int min_bins, double total_weight, double cap) {
    return nr_of_bins > min_bins;
}

bool fully_optimized_prune(int nr_of_bins, int min_bins, double total_weight, double cap) {
    return nr_of_bins > min_bins || nr_of_bins >= total_weight / cap * 2 + 1;
}