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
        vector<Slot> v1(num_blocks);
        it->slots = v1;
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
            int read_hit = 0;
            
            for (int i = 0; i < (int) cache.sets[address_index].slots.size(); i++) {
                if ((cache.sets[address_index].slots[i].valid == true) && (cache.sets[address_index].slots[i].tag == address_tag)) {
                    //read hit
                    read_hit = 1;
                    std::time_t t = std::time(0);
                    cache.sets[address_index].slots[i].time_stamp = (uint32_t) t;
                    load_hits++;
                    break;
                }
            }
            //we have a read miss, attempt to find empty slot
            int full_cache = 1;
            if (read_hit == 0) {
                load_misses++;
                //iterate through
                for (size_t j = 0; j < cache.sets[address_index].slots.size(); j++) {
                    if (cache.sets[address_index].slots[j].valid == false) {
                    //read miss, if there is an empty slot fill it
                        cache.sets[address_index].slots[j].valid = true;
                        cache.sets[address_index].slots[j].tag = address_tag;
                        cache.sets[address_index].slots[j].index = address_index;
                        std::time_t t = std::time(0);
                        cache.sets[address_index].slots[j].time_stamp = (uint32_t) t;
                        full_cache = 0;
                        break;
                    }
                }

                //no empty spaces, find slot to eject
                if (full_cache == 1) {
                    //set first slot as the min
                    uint32_t min_time_stamp = cache.sets[address_index].slots[0].time_stamp;
                    int j_coord = 0;
                    //iterate through to find min using LRU
                  
                    for (size_t j = 0; j < cache.sets[address_index].slots.size(); j++) {
                        
                        if (cache.sets[address_index].slots[j].time_stamp < min_time_stamp) {
                            min_time_stamp = cache.sets[address_index].slots[j].time_stamp;
                            
                            j_coord = j;
                        }
                    }
                    //replace slot
                    cache.sets[address_index].slots[j_coord].index = address_tag;
                    cache.sets[address_index].slots[j_coord].index = address_index;
                    std::time_t t = std::time(0);
                    cache.sets[address_index].slots[j_coord].time_stamp = (uint32_t) t;
                }
            }
        }

        if (operation == "s") {
            total_stores++;
            int hit = 0;
            
            if ((write_through == true) && write_allocate == false) {
                //write through, no-write-allocate
                for (size_t j = 0; j < cache.sets[address_index].slots.size(); j++) {
                    if ((cache.sets[address_index].slots[j].valid == true) && cache.sets[address_index].slots[j].tag == address_tag) {
                        hit = 1;
                        store_hits++;
                        //write immediately to memory
                        stores_to_memory++;
                        break;
                        
                    }
                    
                }

                //no write allocate
                if (hit == 0) {
                    store_misses++;
                    stores_to_memory++;
                }
            }
            if ((write_through == true) && write_allocate == true) {
                int hit = 0;
                //write through, write-allocate
                for (size_t j = 0; j < cache.sets[address_index].slots.size(); j++) {
                    if ((cache.sets[address_index].slots[j].valid == true) && cache.sets[address_index].slots[j].tag == address_tag) {
                        hit = 1;
                        store_hits++;
                        //write immediately to memory
                        stores_to_memory++;
                    }
                    
                }
                    
                if (hit == 0) {
                    store_misses++;
                    stores_to_memory++;
                    int full_cache = 1;
                    //we missed, first check if there is an empty slot to fill
                    
                    for (size_t j = 0; j < cache.sets[address_index].slots.size(); j++) {
                        
                        if (cache.sets[address_index].slots[j].valid == false) {
                            //if there is an empty slot fill it
                            cache.sets[address_index].slots[j].valid = true;
                            cache.sets[address_index].slots[j].tag = address_tag;
                            cache.sets[address_index].slots[j].index = address_index;
                            std::time_t t = std::time(0);
                            cache.sets[address_index].slots[j].time_stamp = (uint32_t) t;
                        
                            full_cache = 0;
                        
                            break;
                        }
                
                    }

                    //slots were full eject least recently used
                    if (full_cache == 1) {
                        uint32_t min_time_stamp = cache.sets[address_index].slots[0].time_stamp;
                        
                        int j_coord = 0;
                       
                        for (size_t j = 0; j < cache.sets[address_index].slots.size(); j++) {
                            
                            
                            if (cache.sets[address_index].slots[j].time_stamp < min_time_stamp) {
                               
                                j_coord = j;
                            }
                            
                                
                
                        }   
                        
                        cache.sets[address_index].slots[j_coord].valid = true;
                        cache.sets[address_index].slots[j_coord].tag = address_tag;
                        cache.sets[address_index].slots[j_coord].index = address_index;
                        std::time_t t = std::time(0);
                        cache.sets[address_index].slots[j_coord].time_stamp = (uint32_t) t;
                    }
                }
            }
            if ((write_through == false) && (write_allocate == true)) {
                //write back and write allocate
                for (size_t j = 0; j < cache.sets[address_index].slots.size(); j++) {
                    if ((cache.sets[address_index].slots[j].valid == true) && cache.sets[address_index].slots[j].tag == address_tag) {
                        hit = 1;
                        store_hits++;
                        cache.sets[address_index].slots[j].dirty = 1;
                        break;
                    }
                }
                    
                if (hit == 0) {
                    store_misses++;
                    stores_to_memory++;
                    //write allocate
                    int full_cache = 1;
                    //we missed, first check if there is an empty slot to fill
                   
                    for (size_t j = 0; j < cache.sets[address_index].slots.size(); j++) {
                        
                        if (cache.sets[address_index].slots[j].valid == false) {
                            //if there is an empty slot fill it
                            cache.sets[address_index].slots[j].valid = true;
                            cache.sets[address_index].slots[j].tag = address_tag;
                            cache.sets[address_index].slots[j].index = address_index;
                            std::time_t t = std::time(0);
                            cache.sets[address_index].slots[j].time_stamp = (uint32_t) t;
                        
                            full_cache = 0;
                           
                            break;
            
                        }
                        

                    }
                       
                    if (full_cache == 1) {
                        //find lru
                        uint32_t min_time_stamp = cache.sets[address_index].slots[0].time_stamp;
                        
                        
                        int j_coord = 0;
                        
                        for (size_t j = 0; j < cache.sets[address_index].slots.size(); j++) {
                            
                            
                            if (cache.sets[address_index].slots[j].time_stamp < min_time_stamp) {
                              
                                j_coord = j;
                            }
                            
                                
                
                        }   

                        
                        
                        cache.sets[address_index].slots[j_coord].valid = true;
                        cache.sets[address_index].slots[j_coord].tag = address_tag;
                        cache.sets[address_index].slots[j_coord].index = address_index;
                        std::time_t t = std::time(0);
                        cache.sets[address_index].slots[j_coord].time_stamp = (uint32_t) t;
                        
                        //check if ejected slot was dirty and needs to be written to memory
                        if (cache.sets[address_index].slots[j_coord].dirty == 1) {
                            stores_to_memory++;
                        }

                    }

                }



            }
            
        }
    }

    total_cycles = 4*load_hits+(100*load_misses*num_bytes/4)+(4*store_hits)+(100*stores_to_memory*num_bytes/4);
    cout << "Total loads: " << total_loads << endl;
    cout << "Total stores: " << total_stores << endl;
    cout << "Load hits: " << load_hits << endl;
    cout << "Load misses: " << load_misses << endl;
    cout << "Store hits: "<< store_hits << endl;
    cout << "Store misses: " << store_misses <<endl;
    cout << "Total cycles: " << total_cycles << endl;
    return 0;
}
