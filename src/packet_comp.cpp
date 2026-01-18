#include "bin_packing.h"

bool compare_packs_decr(packet A, packet B) {
    if(A.weight > B.weight)
        return 1;
    else if(A.weight < B.weight)
        return 0;
    else if(A.index < B.index)
        return 1;
    else if(A.index > B.index)
        return 0;
    return 1;
}