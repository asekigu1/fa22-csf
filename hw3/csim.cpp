#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <bitset>
#include "helper_functions.h"


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
    int memory_access = 0;
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
        uint32_t address_index = get_index(address, num_sets, num_bytes, num_blocks);
        if (num_sets == 1) {
            address_index = 0;
        }

        if (operation == "l") {
            total_loads++;

            // check if there are any hits
            int hit = -1;
            hit = check_hit(&cache, address_index, address_tag, &load_hits, lru, total_loads, total_stores);

            //miss 
            if (hit == -1) {
                load_misses++;
                int complete = -1;
                complete = insert_if_cache_not_full(&cache, address_index, address_tag, &memory_access, total_loads, total_stores);

                //find block to eject
                if (complete == -1) {
                    // find the least recently used, OR first-in
                    int index = find_oldest(&cache, address_index);

                    if (cache.sets[address_index].blocks[index].dirty && !write_through) {
                        // if write-back and the block is dirty, write to memory
                        memory_access = memory_access * (num_bytes/4);
                    }

                    //replace slot
                    new_cache(&cache, address_index, address_tag, index, &memory_access, total_loads, total_stores);
                }
            }
        }

        if (operation == "s") {
            total_stores++;
            

            //check for hit
            int hit = -1;
            hit = check_hit(&cache, address_index, address_tag, &store_hits, lru, total_loads, total_stores);
            if (hit != -1) {
                cache.sets[address_index].blocks[hit].dirty = true; //write to cache
                if (write_through) {
                    //write to memory

                    memory_access++;
                }
            }
            
                
            
            if (hit == -1) {
                store_misses++;

                if (write_allocate) {
                    //iterate through to insert in empty block
                    int complete = -1;
                    complete = insert_if_cache_not_full(&cache, address_index, address_tag, &memory_access, total_loads, total_stores);
                    if (complete != -1) { // if it successfully loaded
                        cache.sets[address_index].blocks[hit].dirty = true; //write to cache
                        if (write_through) {
                            //write to memory
                            memory_access++;
                        }
                    } else //find block to eject
                     {
                        // find the least recently used, OR first-in
                        int index = find_oldest(&cache, address_index);

                        if (cache.sets[address_index].blocks[index].dirty && !write_through) {
                            // if write-back and the block is dirty, write to memory
                            memory_access = memory_access * (num_bytes/4);
                        }

                        //replace slot
                        new_cache(&cache, address_index, address_tag, index, &memory_access, total_loads, total_stores);
                        if (write_through) {
                            // only store to memory for write_through
                            memory_access++;
                        } else {
                            cache.sets[address_index].blocks[index].dirty = true;
                        }
                    }
                    
                } else {
                    // for no-write-allocate, don't modify cache for miss
                    // directly store to memory
                    memory_access++;
                }
            }
            
        }
    }

    total_cycles = load_hits + store_hits +(100*memory_access);
    cout << "Total loads: " << total_loads << endl;
    cout << "Total stores: " << total_stores << endl;
    cout << "Load hits: " << load_hits << endl;
    cout << "Load misses: " << load_misses << endl;
    cout << "Store hits: "<< store_hits << endl;
    cout << "Store misses: " << store_misses <<endl;
    cout << "Total cycles: " << total_cycles << endl;
    return 0;
}
