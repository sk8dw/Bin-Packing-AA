#include <iostream>
#include <fstream>
#include <cstring>
#include "bin_packing.h"
#include "benchmark/benchmark.h"

using namespace std;

struct Tests {
    string name;
    int cap;
    vector<packet> packs;
    double total_weight;
};

vector<Tests> tests;

void load_from_path(char* path) {
    string full_path[100] = {0};
    for(int i = 0; i < 25 ; i++) {
        strcpy(full_path, path);
        str
        ifstream fin(path + i +".txt");

    }
}

void Load_tests() {
    char path[50] = {0};
    strcpy(path,"tests\\near-half\\test\0");

}


