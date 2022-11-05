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
    int total_loads = 0;
    int total_stores = 0;
    int load_hits = 0;
    int load_misses = 0;
    int store_hits = 0;
    int store_misses = 0;
    int memory_access = 0;
    int total_cycles = 0;
    string line;

    if (validate_input(argc, argv, &num_sets, &num_blocks, &num_bytes, &write_allocate, &write_through, &lru)) {
        return 1; // return non-zero exitcode if invalid
    }
    
    // initialize cache
    Cache cache;
    vector<Set> v(num_sets);
    cache.sets = v;
    for (vector<Set>::iterator it = cache.sets.begin(); it != cache.sets.end(); ++it) {
        vector<Block> v1(num_blocks);
        it->blocks = v1;
    }
    
    // go through all traces
    while (std::getline(cin, line)){
        stringstream ss(line);
        // save first field
        string operation;
        ss >> operation;
        
        // save address
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
            // load
            total_loads++;

            // check if there are any hits, return index of hit block
            int hit_ind = -1;
            hit_ind = check_hit(&cache, address_index, address_tag, &load_hits, lru, total_loads, total_stores);

            //miss 
            if (hit_ind == -1) {
                load_misses++;

                // check if the cache is full. load to an empty block if it exists, return the index.
                int load_ind = -1;
                load_ind = insert_if_cache_not_full(&cache, address_index, address_tag, &memory_access, total_loads, total_stores, num_bytes);

                // if the cache is full, find block to eject
                if (load_ind == -1) {
                    // find the least recently used, OR first-in
                    int index = find_oldest(&cache, address_index);

                    if (cache.sets[address_index].blocks[index].dirty && !write_through) {
                        // if write-back and the block is dirty, write the entire block to memory
                        memory_access += (num_bytes/4);
                    }

                    // replace slot with new block
                    new_cache(&cache, address_index, address_tag, index, &memory_access, total_loads, total_stores, num_bytes);
                }
            }
        }

        if (operation == "s") {
            // store
            total_stores++;
            
            //check for hit
            int hit_ind = -1;
            hit_ind = check_hit(&cache, address_index, address_tag, &store_hits, lru, total_loads, total_stores);
            if (hit_ind != -1) {
                cache.sets[address_index].blocks[hit_ind].dirty = true; //write to cache
                if (write_through) {
                    // write the 4bytes data to memory
                    memory_access++;
                }
            }
            
            if (hit_ind == -1) {
                // store miss
                store_misses++;

                if (write_allocate) {
                    // check if the cache is full. store to an empty block if it exists, return the index.
                    int store_ind = -1;
                    store_ind = insert_if_cache_not_full(&cache, address_index, address_tag, &memory_access, total_loads, total_stores, num_bytes);
                    if (store_ind != -1) { // if it successfully loaded
                        cache.sets[address_index].blocks[store_ind].dirty = true; //write to cache
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
                            memory_access += (num_bytes/4);
                        }

                        // replace slot with new block
                        new_cache(&cache, address_index, address_tag, index, &memory_access, total_loads, total_stores, num_bytes);
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

    // additional stats for part(b)
    float rate = (load_hits + store_hits)/ (float)(total_loads + total_stores);
    cout << "Hit Rate (%): " << rate*100 << endl;
    cout << "Total cycles: " << total_cycles << endl;
    cout << "Total Cache Size: " << (num_sets*num_blocks*num_bytes) << endl;
    int overhead = 0;
    if (write_allocate && write_through) {
        overhead = (sizeof(bool) + sizeof(int)) * (num_sets*num_blocks);
    } else if (write_allocate && !write_through) {
        overhead = (sizeof(bool)*2 + sizeof(int)) * (num_sets*num_blocks);
    } else if (!write_allocate && write_through) {
        overhead = (sizeof(bool) + sizeof(int)) * (num_sets*num_blocks);
    }
    cout << "Overhead cache size: " << overhead << endl;
    return 0;
}
