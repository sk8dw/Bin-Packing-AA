#include "bin_packing.h"

vector<bin> best_fit(int n, int cap, vector<packet> pack) {
    vector<bin> bins;
    bool fit;
    int best_index;
    int remaining_space_after_fit;
    for(int i = 0; i < n ; i++) {
        fit = false;
        best_index = -1;
        remaining_space_after_fit = cap + 1;
        for(size_t j = 0; j < bins.size(); j++) {
            if(bins[j].space_left == pack[i].weight) {
                fit = true;
                best_index = j;
                remaining_space_after_fit = 0;
                break;
            } else if(bins[j].space_left > pack[i].weight && bins[j].space_left - pack[i].weight < remaining_space_after_fit) {
                remaining_space_after_fit = bins[j].space_left - pack[i].weight;
                best_index = j;
                fit = true;
            }
        }
        if(fit == false) {
            bins.push_back({cap - pack[i].weight,{pack[i]}});
        } else {
            bins[best_index].space_left -= pack[i].weight;
            bins[best_index].packets.push_back(pack[i]);
        }
    }
    return bins;
}