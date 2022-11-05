#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <bitset>
#include "helper_functions.h"
#include <ctime>


using std::cout; using std::cin; using std::endl;
using std::cerr; using std::vector; using std::string; using std::stringstream;

int main(int argc, char * argv[]) {
    
    int num_sets = 0;
    int num_blocks = 0;
    int num_bytes = 0;
    bool write_allocate;
    bool write_through;
    bool lru;

    if (validate_input(argc, argv, &num_sets, &num_blocks, &num_bytes, &write_allocate, &write_through, &lru)) {
        return 1;
        // return non-zero exitcode if invalid
    }
    
    // initialize cache
    Cache cache;
    vector<Set> v(num_sets);
    cache.sets = v;
    for (vector<Set>::iterator it = cache.sets.begin(); it != cache.sets.end(); ++it) {
        vector<Block> v1(num_blocks);
        it->blocks = v1;
    }
    
    int total_loads = 0;
    int total_stores = 0;
    int load_hits = 0;
    int load_misses = 0;
    int store_hits = 0;
    int store_misses = 0;
    int stores_to_memory = 0;
    int total_cycles = 0;

    string line;
    while (std::getline(cin, line)){
        stringstream ss(line);
        // first field
        string operation;
        ss >> operation;


        // address
        string address_str;
        ss >> address_str;
        stringstream s2;
        s2 << std::hex << address_str;
        uint32_t address;
        s2 >> address;
        uint32_t address_tag = get_tag(address, num_sets, num_bytes);
        uint32_t address_index = get_index(address, num_sets, num_bytes);
        if (num_sets == 1) {
            address_index = 0;
        }


        if (operation == "l") {
            total_loads++;
            
            
        }

        if (operation == "s") {
            total_stores++;
            
            if ((write_through == true) && write_allocate == false) {
            
            }
            if ((write_through == true) && write_allocate == true) {
            }
            if ((write_through == false) && (write_allocate == true)) {

            }
            
        }
    }

    total_cycles = ;
    cout << "Total loads: " << total_loads << endl;
    cout << "Total stores: " << total_stores << endl;
    cout << "Load hits: " << load_hits << endl;
    cout << "Load misses: " << load_misses << endl;
    cout << "Store hits: "<< store_hits << endl;
    cout << "Store misses: " << store_misses <<endl;
    cout << "Total cycles: " << total_cycles << endl;
    return 0;
}
