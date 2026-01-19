#include "bin_packing.h"

vector<bin> best_fit_decreasing(int cap, vector<packet> pack) {
    sort(pack.begin(), pack.end(),compare_packs_decr);
    return best_fit(cap,pack);
}