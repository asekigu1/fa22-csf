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
    
    // validate arguments
    if (argc != 6 && argc != 7) { 
        cerr << "Invalid number of arguments" << endl;
        return 1;
    }
   
    //  validate number of sets is a positive power of 2
    int num_sets = atoi(argv[1]);
    int num_blocks = atoi(argv[2]);
    int num_bytes = atoi(argv[3]);
    
    if ((is_power_of_2(num_sets) == 0) || (is_power_of_2(num_blocks) == 0 || (num_blocks < 4)))  {
        cerr << "Invalid parameter in validation of num_sets to num_bytes";
        return 1;
    }
    
    
    //  validate write-allocate and write-through modes
    bool write_allocate;
    bool write_through;
    
    if (strcmp(argv[4],"write-allocate") == 0) {
        write_allocate = true;
    } else if (strcmp(argv[4],"no-write-allocate") == 0) {
        write_allocate = false;
    } else {
        cerr << "Invalid parameter in write-allocate";
        return 1;
    }
    if (strcmp(argv[5],"write-through") == 0) {
        write_through = true;
    } else if (strcmp(argv[5], "write-back") == 0) {
        write_through = false;
    } else {
        cerr << "Invalid parameter in write-through";
        return 1;
    }
    if (!write_allocate && !write_through) {
        cerr << "Cannot pass no-write-allocate and write-back";
        return 1;
    }
    
    //  validate eviction
    bool lru;
    if (strcmp(argv[6],"lru") == 0) {
        lru = true;
    } else if (strcmp(argv[6],"fifo") == 0) {
        lru = false;
    } else {
        cerr << "Invalid parameter in eviction";
        return 1;
    }
    
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
    
    

    string line;
    
    while (std::getline(cin, line)){
        //get if load or store
        string operation;
        stringstream ss(line);
        ss >> operation;
        //get the address
        //cout << operation << endl;
        string address;
        ss >> address;
        //convert to number
        stringstream s2;
        s2 << std::hex << address;
        uint32_t num;
        s2 >> num;
        uint32_t address_tag= get_tag(num, num_sets, num_blocks);
        uint32_t address_index = get_index(num, num_sets, num_blocks);
            

        if (operation == "l") {
            //when we have a load
            total_loads++;
            int read_hit = 0;
            //iterate through sets and slots
            for (size_t i = 0; i < cache.sets.size(); i++) {
                
                for (size_t j = 0; j < cache.sets[i].slots.size(); j++) {
                    Slot current_slot = cache.sets[i].slots[j];
                    
                    if ((current_slot.valid == true) && (current_slot.tag == address_tag)) {
                        //if we have a read hit
                        current_slot.index = address_index;
                        read_hit = 1;
                        std::time_t t = std::time(0);
                        current_slot.time_stamp = (uint32_t) t;
                        load_hits++;


                    }

                
                }
            }
            //we have a read miss, attempt to find empty slot
            int full_cache = 1;
            if (read_hit == 0) {
                load_misses++;
                //iterate through
                for (size_t i = 0; i < cache.sets.size(); i++) {
                    for (size_t j = 0; j < cache.sets[i].slots.size(); j++) {
                        Slot current_slot = cache.sets[i].slots[j];
                
                        if (current_slot.valid == false) {
                        //read miss, if there is an empty slot fill it
                            current_slot.valid = true;
                            current_slot.tag = address_tag;
                            current_slot.index = address_index;
                            std::time_t t = std::time(0);
                            current_slot.time_stamp = (uint32_t) t;
                            
                            full_cache = 0;
                    


                        }
                }   }
            
            }
            //no empty spaces, find slot to eject
            if (full_cache == 1) {
                //set first slot as the min
                uint32_t min_time_stamp = cache.sets[0].slots[0].time_stamp;
                int i_coord;
                int j_coord;
                //iterate through to find min using LRU
                for (size_t i = 0; i < cache.sets.size(); i++) {
                    for (size_t j = 0; j < cache.sets[i].slots.size(); j++) {
                        Slot current_slot = cache.sets[i].slots[j];
                        if (current_slot.time_stamp < min_time_stamp) {
                            min_time_stamp = current_slot.time_stamp;
                            i_coord = i;
                            j_coord = j;
                        }
                    }

                } 
                //replace slot
                cache.sets[i_coord].slots[j_coord].index = address_tag;
                cache.sets[i_coord].slots[j_coord].index = address_index;
                std::time_t t = std::time(0);
                cache.sets[i_coord].slots[j_coord].time_stamp = (uint32_t) t;


                
                        
            


            }
        if (operation == "s") {
            total_stores++;
            int hit = 0;
            //write through, no-write-allocate
            if ((write_through == true) && write_allocate == false) {
                for (size_t i = 0; i < cache.sets.size(); i++) {
                    for (size_t j = 0; j < cache.sets[i].slots.size(); j++) {
                        Slot current_slot = cache.sets[i].slots[j];
                        if ( (current_slot.valid == true) && current_slot.tag == address_tag && current_slot.index == address_index) {
                            hit = 1;
                            store_hits++;
                            //write immediately to memory
                            stores_to_memory++;
                        }
                    }

                }

                //no write allocate
            
                if (hit == 0) {
                    store_misses++;
                    stores_to_memory++;
                }
            }
            if ((write_through == true) && write_allocate == true) {
                total_stores++;
                int hit = 0;
                //write through, write-allocate
                for (size_t i = 0; i < cache.sets.size(); i++) {
                    for (size_t j = 0; j < cache.sets[i].slots.size(); j++) {
                        Slot current_slot = cache.sets[i].slots[j];
                        if ( (current_slot.valid == true) && current_slot.tag == address_tag && current_slot.index == address_index) {
                            hit = 1;
                            store_hits++;
                            //write immediately to memory
                            stores_to_memory++;
                        }
                    }

                }
                if (hit == 0) {
                    store_misses++;
                    int full_cache = 1;
                    //we missed, first check if there is an empty slot to fill
                    for (size_t i = 0; i < cache.sets.size(); i++) {
                        for (size_t j = 0; j < cache.sets[i].slots.size(); j++) {
                            Slot current_slot = cache.sets[i].slots[j];
                            if (current_slot.valid == false) {
                            //if there is an empty slot fill it
                            current_slot.valid = true;
                            current_slot.tag = address_tag;
                            current_slot.index = address_index;
                            std::time_t t = std::time(0);
                            current_slot.time_stamp = (uint32_t) t;
                        
                            full_cache = 0;
                
                            }

                        }
                    }
                    //slots were full eject least recently used
                    if (full_cache == 1) {
                        uint32_t min_time_stamp = cache.sets[0].slots[0].time_stamp;
                        
                        for (size_t i = 0; i < cache.sets.size(); i++) {
                            for (size_t j = 0; j < cache.sets[i].slots.size(); j++) {
                                
                                Slot current_slot = cache.sets[i].slots[j];
                                if (current_slot.time_stamp < min_time_stamp) {
                                    //if there is an empty slot fill it
                                    current_slot.valid = true;
                                    current_slot.tag = address_tag;
                                    current_slot.index = address_index;
                                    std::time_t t = std::time(0);
                                    current_slot.time_stamp = (uint32_t) t;
                                }
                                
                                    
                    
                            }   

                        }
                    }

                    

                }

            }
            if ((write_through == false) && (write_allocate == true)) {
                //write back and write allocate

                //hit case
                total_stores++;
                int hit = 0;
                //write through, write-allocate
                for (size_t i = 0; i < cache.sets.size(); i++) {
                    for (size_t j = 0; j < cache.sets[i].slots.size(); j++) {
                        Slot current_slot = cache.sets[i].slots[j];
                        if ( (current_slot.valid == true) && current_slot.tag == address_tag && current_slot.index == address_index) {
                            hit = 1;
                            store_hits++;
                            current_slot.dirty = 1;
                        }
                    }

                }
                if (hit == 0) {
                    store_misses++;
                    //write allocate
                    int full_cache = 1;
                    //we missed, first check if there is an empty slot to fill
                    for (size_t i = 0; i < cache.sets.size(); i++) {
                        for (size_t j = 0; j < cache.sets[i].slots.size(); j++) {
                            Slot current_slot = cache.sets[i].slots[j];
                            if (current_slot.valid == false) {
                                //if there is an empty slot fill it
                                current_slot.valid = true;
                                current_slot.tag = address_tag;
                                current_slot.index = address_index;
                                std::time_t t = std::time(0);
                                current_slot.time_stamp = (uint32_t) t;
                            
                                full_cache = 0;
                
                            }

                        }
                    }
                    if (full_cache == 1) {
                        //find lru
                        uint32_t min_time_stamp = cache.sets[0].slots[0].time_stamp;
                        Slot ejected_slot;
                        for (size_t i = 0; i < cache.sets.size(); i++) {
                            for (size_t j = 0; j < cache.sets[i].slots.size(); j++) {
                                
                                Slot current_slot = cache.sets[i].slots[j];
                                if (current_slot.time_stamp < min_time_stamp) {
                                    //if there is an empty slot fill it
                                    ejected_slot = current_slot;
                                    current_slot.valid = true;
                                    current_slot.tag = address_tag;
                                    current_slot.index = address_index;
                                    std::time_t t = std::time(0);
                                    current_slot.time_stamp = (uint32_t) t;
                                    
                                }
                                
                                    
                    
                            }   

                        }
                        //check if ejected slot was dirty and needs to be written to memory
                        if (ejected_slot.dirty == 1) {
                            stores_to_memory++;
                        }

                    }

                }



            }
            
            



        
        
        }

    
    
        
    
    }


            
            
        
            
            
            
    }
    cout << "Total loads: " << total_loads << endl;
    cout << "Total stores: " << total_stores << endl;
    cout << "Load hits: " << load_hits << endl;
    cout << "Load misses: " << load_misses << endl;
    cout << "Store hits: "<< store_hits << endl;
    cout << "Store misses: " << store_misses <<endl;
    cout << "Total cycles: " << endl;





  
    
  

    return 0;
}
