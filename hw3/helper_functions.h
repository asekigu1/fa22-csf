#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <bitset>
#include <map>
#include <climits>

using std::cout; using std::cin; using std::endl;
using std::cerr; using std::vector; using std::string; using std::map;

struct Block {
    uint32_t tag = 0;
    uint32_t index = 0;
    bool valid = false;
    int time_stamp = INT_MAX;
    bool dirty = false;
};

struct Set {
    vector<Block> blocks;
};

struct Cache {
    vector<Set> sets;
};

int is_power_of_2(int n);
uint32_t get_tag(uint32_t num, uint32_t set_size, uint32_t block_size);
uint32_t get_index(uint32_t num, uint32_t set_size, uint32_t block_size);
int validate_input(int argc, char * argv[], int* num_sets, int* num_blocks, int* num_bytes, bool* write_allocate, bool* write_through, bool* lru);
int check_hit(Cache* cache, uint32_t address_index, uint32_t address_tag, int* hits, bool lru, int total_loads, int total_stores);
int new_cache(Cache* cache, uint32_t address_index, uint32_t address_tag, int block_ind, int* memory, int total_loads, int total_stores, int num_bytes);
int find_oldest(Cache* cache, uint32_t address_index);
int insert_if_cache_not_full(Cache* cache, uint32_t address_index, uint32_t address_tag, int* memory, int total_loads, int total_stores, int num_bytes);