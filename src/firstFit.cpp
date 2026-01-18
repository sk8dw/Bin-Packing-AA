#include "bin_packing.h"

vector<bin> first_fit(int n, int cap, vector<packet> pack) {
    vector<bin> bins;
    bool fit;
    for(int i = 0; i < n ; i++) {
        fit = false;
        for(size_t j = 0; j < bins.size(); j++) {
            if(bins[j].space_left >= pack[i].weight) {
                fit = true;
                bins[j].space_left -= pack[i].weight;
                bins[j].packets.push_back(pack[i]);
                break;
            }
        }
        if(fit == false) {
            bins.push_back({cap - pack[i].weight,{pack[i]}});
        }
    }
    return bins;
}