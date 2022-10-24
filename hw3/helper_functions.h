#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <bitset>
#include <map>

using std::cout; using std::cin; using std::endl;
using std::cerr; using std::vector; using std::string; using std::map;

struct Slot {
    uint32_t tag;
    uint32_t index;
    bool valid = false;
    uint32_t time_stamp;
    int dirty = 0;

};

struct Set {
    vector<Slot> slots;
    std::map<unsigned, unsigned> index; // map of tag to index of slot
};

struct Cache {
    vector<Set> sets;
};


int is_power_of_2(int n);

string convert_hex_to_binary (string hex);

uint32_t get_tag(uint32_t num, uint32_t set_size, uint32_t block_size);

uint32_t get_index(uint32_t num, uint32_t set_size, uint32_t block_size);