#include "bin_packing.h"

vector<bin> first_fit_decreasing(int n, int cap, vector<packet> pack) {
    sort(pack.begin(), pack.end(),compare_packs_decr);
    return first_fit(n,cap,pack);
}
