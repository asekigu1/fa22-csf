#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <bitset>
#include "helper_functions.h"
using std::cout; using std::cin; using std::endl;
using std::cerr; using std::vector; using std::string;

int is_power_of_2(int n){
    while (n % 2 == 0) {
        n = n /2;
    }
    //n is now odd, if n=1 then it is a power of 2
    if (n == 1){
        return 1;
    }
    return 0;
}

int log_2(int num) {
    int count = 0;
    while (num != 1) {
        num = num /2;
        count++;
    }
    return count;
}

uint32_t get_tag(uint32_t num, uint32_t set_size, uint32_t block_size) {
    uint32_t log2_set_size = log_2(set_size);
    uint32_t log2_block_size = log_2(block_size);
    return num >> (log2_set_size + log2_block_size);
}

uint32_t get_index(uint32_t num, uint32_t set_size, uint32_t block_size) {
    uint32_t log2_set_size = log_2(set_size);
    uint32_t log2_block_size = log_2(block_size);
    uint32_t digits = 32 - log2_block_size - log2_set_size;
    num = num << digits;
    num = num >> (digits + log2_block_size);
    return num;
}

int validate_input(int argc, char * argv[], int* num_sets, int* num_blocks, int* num_bytes, bool* write_allocate, bool* write_through, bool* lru) {
    if (argc != 7) { 
        cerr << "Invalid number of arguments" << endl;
        return 1;
    }

    *num_sets = atoi(argv[1]);
    if (!is_power_of_2(*num_sets)) {
        cerr << "Set number is not a power of 2" << endl;
        return 1;
    }

    *num_blocks = atoi(argv[2]);
    if (!is_power_of_2(*num_blocks)) {
        cerr << "Block number is not a power of 2" << endl;
        return 1;
    }

    *num_bytes = atoi(argv[3]);
    if (!is_power_of_2(*num_bytes)) {
        cerr << "Byte number is not a power of 2" << endl;
        return 1;
    }
    if (*num_bytes < 4) {
        cerr << "Byte number is less than 4" << endl;
        return 1;
    }

    //  validate write-allocate and write-through modes
    if (strcmp(argv[4],"write-allocate") == 0) {
        *write_allocate = true;
    } else if (strcmp(argv[4],"no-write-allocate") == 0) {
        *write_allocate = false;
    } else {
        cerr << "Invalid parameter. Cache Miss Mode must be 'write-allocate' or 'no-write-allocate'.";
        return 1;
    }

    if (strcmp(argv[5],"write-through") == 0) {
        *write_through = true;
    } else if (strcmp(argv[5], "write-back") == 0) {
        *write_through = false;
    } else {
        cerr << "Invalid parameter. Store Mode must be 'write-through' or 'write-back'.";
        return 1;
    }

    if (!(*write_allocate) && !(*write_through)) {
        cerr << "no-write-allocate and write-back is an invalid combination";
        return 1;
    }

    //  validate eviction
    if (strcmp(argv[6],"lru") == 0) {
        *lru = true;
    } else if (strcmp(argv[6],"fifo") == 0) {
        *lru = false;
    } else {
        cerr << "Invalid parameter. Eviction Mode must be lru or fifo.";
        return 1;
    }

    return 0;
}

