#include <iostream>
#include <cstdio>
#include "bin_packing.h"
using namespace std;

void print_sols_all(vector<vector<bin>> sols);
void print_sol(vector<bin> sol);
vector<packet> convert_to_pack(int weights[], int n);
int main() {
    int n, cap, total_weight=0;
    bool valid = true;
    int weight[1000];
    ///do input reading and input validation + weight calculation
    cout << "Introduceti pe rand numarul de pachete, capacitatea si apoi greutatea pachetelor:\n";
    cin >> n >> cap;
    for(int i = 0; i < n; i++) {
        cin >> weight[i];
        if(weight[i] > cap) 
            valid = false;
        total_weight += weight[i];
    }
    if(!valid) {
        cout << "Invalid input!! The weight of all pachets must be smaller than the capacity of a bin!\n";
    } else {
        vector<vector<bin>> sols = backtracking_all(n,cap,weight,fully_optimized_prune,total_weight);
        print_sols_all(sols);
        printf("---------------------------------\n");
        vector<packet> packs = convert_to_pack(weight,n);
        vector<bin> ffdsol = first_fit_decreasing(n,cap,packs);
        print_sol(ffdsol);
        printf("---------------------------------\n");
        vector<bin> ffsol = first_fit(n,cap,packs);
        print_sol(ffsol);
        printf("---------------------------------\n");
        vector<bin> bfdsol = best_fit_decreasing(n,cap,packs);
        print_sol(bfdsol);
        printf("---------------------------------\n");
        vector<bin> bfsol = best_fit(n,cap,packs);
        print_sol(bfsol);
        printf("---------------------------------\n");
    }
    return 0;
}

void print_sols_all(vector<vector<bin>> sols) {
    for(size_t i = 0; i < sols.size(); i++) {
        cout << "Solution " << i << endl;
        for(size_t j = 0; j < sols[i].size(); j++) {
            cout << "\tBin " << j << ": ";
            for(size_t k = 0; k < sols[i][j].packets.size(); k++) {
                cout<< "(" << sols[i][j].packets[k].index << "," << sols[i][j].packets[k].weight << ") ";
            }
            cout << endl;
        }
    }
}

void print_sol(vector<bin> sol) {
    for(size_t j = 0; j < sol.size(); j++) {
            cout << "Bin " << j << ": ";
            for(size_t k = 0; k < sol[j].packets.size(); k++) {
                cout<< "(" << sol[j].packets[k].index << "," << sol[j].packets[k].weight << ") ";
            }
            cout << endl;
        }
}

vector<packet> convert_to_pack(int weights[], int n) {
    vector<packet> packs;
    for(int i = 0; i < n; i++) {
        packs.push_back({weights[i],i});
    }
    return packs;
}