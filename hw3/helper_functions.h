#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <bitset>

using std::cout; using std::cin; using std::endl;
using std::cerr; using std::vector; using std::string;

struct Slot {
    uint32_t tag;
    uint32_t index;
    bool valid;
    uint32_t time_stamp;

};

struct Set {
    vector<Slot> slots;
};

struct Cache {
    vector<Set> sets;
};


int is_power_of_2(int n);

string convert_hex_to_binary (string hex);

uint32_t get_tag(uint32_t num, uint32_t set_size, uint32_t block_size);

uint32_t get_index(uint32_t num, uint32_t set_size, uint32_t block_size);