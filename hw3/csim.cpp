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
            bool hit = false;
            bool complete = false;
            
            //check for hit
            for (int i = 0; i < num_blocks; i++) {
                if ((cache.sets[address_index].blocks[i].valid == true) && (cache.sets[address_index].blocks[i].tag == address_tag)) {
                    hit = true;
                    load_hits++;

                    if (lru) {
                        // only update time stamp for lru
                        std::time_t t = std::time(0);
                        cache.sets[address_index].blocks[i].time_stamp = (uint32_t) t;
                    }
                    
                    break;
                }
            }

            //miss 
            if (!hit) {
                load_misses++;
                //iterate through to insert in empty block
                for (int i = 0; i < num_blocks; i++) {
                    if (cache.sets[address_index].blocks[i].valid == false) {
                        //add to empty block
                        cache.sets[address_index].blocks[i].tag = address_tag;
                        cache.sets[address_index].blocks[i].index = address_index;
                        cache.sets[address_index].blocks[i].valid = true;
                        std::time_t t = std::time(0);
                        cache.sets[address_index].blocks[i].time_stamp = (uint32_t) t;
                        cache.sets[address_index].blocks[i].dirty = false;
                        complete = true;
                        break;
                    }
                }

                //find block to eject
                if (!complete) {
                    
                    uint32_t min_time_stamp = cache.sets[address_index].blocks[0].time_stamp;
                    int index = 0;

                    //iterate through to find minimum time stamp
                    for (int i = 0; i < num_blocks; i++) {
                        if (cache.sets[address_index].blocks[i].time_stamp < min_time_stamp) {
                            min_time_stamp = cache.sets[address_index].blocks[i].time_stamp;
                            index = i;
                        }
                    }

                    if (cache.sets[address_index].blocks[index].dirty && !write_through) {
                        // if write-back and the block is dirty, write to memory
                        stores_to_memory++;
                    }

                    //replace slot
                    cache.sets[address_index].blocks[index].tag = address_tag;
                    cache.sets[address_index].blocks[index].index = address_index;
                    cache.sets[address_index].blocks[index].valid = true;
                    std::time_t t = std::time(0);
                    cache.sets[address_index].blocks[index].time_stamp = (uint32_t) t;
                    cache.sets[address_index].blocks[index].dirty = false;
                }
            }
        }

        if (operation == "s") {
            total_stores++;
            bool hit = false;
            bool complete = false;

            //check for hit
            for (int i = 0; i < num_blocks; i++) {
                if ((cache.sets[address_index].blocks[i].valid == true) && (cache.sets[address_index].blocks[i].tag == address_tag)) {
                    hit = true;
                    store_hits++;

                    if (lru) {
                        // only update time stamp for lru
                        std::time_t t = std::time(0);
                        cache.sets[address_index].blocks[i].time_stamp = (uint32_t) t;
                    }

                    if (write_through) {
                        //write to memory directly
                        stores_to_memory++;
                    } else {
                        //write to cache
                        cache.sets[address_index].blocks[i].dirty = true;
                    }
                    
                    break;
                }
            }
            
            if (!hit) {
                store_misses++;

                if (write_allocate) {
                    //iterate through to insert in empty block
                    for (int i = 0; i < num_blocks; i++) {
                        if (cache.sets[address_index].blocks[i].valid == false) {
                            //add to empty block
                            cache.sets[address_index].blocks[i].tag = address_tag;
                            cache.sets[address_index].blocks[i].index = address_index;
                            cache.sets[address_index].blocks[i].valid = true;
                            std::time_t t = std::time(0);
                            cache.sets[address_index].blocks[i].time_stamp = (uint32_t) t;
                            cache.sets[address_index].blocks[i].dirty = true;
                            complete = true;
                            
                            if (write_through) {
                                // only store to memory for write_through
                                stores_to_memory++;
                            }
                            break;
                        }
                    }

                    //find block to eject
                    if (!complete) {
                        uint32_t min_time_stamp = cache.sets[address_index].blocks[0].time_stamp;
                        int index = 0;
                        //iterate through to find minimum time stamp
                        for (int i = 0; i < num_blocks; i++) {
                            if (cache.sets[address_index].blocks[i].time_stamp < min_time_stamp) {
                                min_time_stamp = cache.sets[address_index].blocks[i].time_stamp;
                                index = i;
                            }
                        }

                        if (cache.sets[address_index].blocks[index].dirty && !write_through) {
                            // if write-back and the block is dirty, write to memory
                            stores_to_memory++;
                        }

                        //replace slot
                        cache.sets[address_index].blocks[index].tag = address_tag;
                        cache.sets[address_index].blocks[index].index = address_index;
                        cache.sets[address_index].blocks[index].valid = true;
                        std::time_t t = std::time(0);
                        cache.sets[address_index].blocks[index].time_stamp = (uint32_t) t;
                        cache.sets[address_index].blocks[index].dirty = true;
                        if (write_through) {
                            // only store to memory for write_through
                            stores_to_memory++;
                        }
                    }
                    
                } else {
                    // for no-write-allocate, don't modify cache for miss
                    // directly store to memory
                    stores_to_memory++;
                }
            }
            
        }
    }

    total_cycles = load_hits + store_hits + (100*load_misses*num_bytes/4)+(100*stores_to_memory*num_bytes/4);
    cout << "Total loads: " << total_loads << endl;
    cout << "Total stores: " << total_stores << endl;
    cout << "Load hits: " << load_hits << endl;
    cout << "Load misses: " << load_misses << endl;
    cout << "Store hits: "<< store_hits << endl;
    cout << "Store misses: " << store_misses <<endl;
    cout << "Total cycles: " << total_cycles << endl;
    return 0;
}
