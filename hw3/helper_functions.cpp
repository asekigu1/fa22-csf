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
    //while n is even keep on dividing by 2
    while (n % 2 == 0) {
        n = n /2;
        //cout << n << endl;
    }
    //n is now odd, if it is 1 then it is a power of 2, any other odd number means it wasn't
    if (n == 1){
        return 1;
    }
    else {
        return 0;
    }

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
    num = num >> (log2_set_size+log2_block_size); 
    return num;
}

uint32_t get_index(uint32_t num, uint32_t set_size, uint32_t block_size) {
    uint32_t log2_set_size = log_2(set_size);
    uint32_t log2_block_size = log_2(block_size);
    uint32_t tag_digits = 32-log2_block_size-log2_set_size;
    num = num << tag_digits;
    num = num >> (tag_digits+log2_block_size);
    return num;
    
    

}

